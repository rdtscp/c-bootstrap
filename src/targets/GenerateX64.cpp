#include "targets/GenerateX64.h"

using namespace ACC;

GenerateX64::GenerateX64(atl::shared_ptr<Program> progAST)
    : progAST(progAST), outputSource(new SourceMemHandler()),
      x64(outputSource) {}

void GenerateX64::printErrors() const {
  printf("FATAL x64 Generation Errors:\n");
  for (unsigned int idx = 0; idx < errors.size(); ++idx)
    printf("\t%s\n", errors[idx].c_str());
}

atl::shared_ptr<SourceMemHandler> GenerateX64::run() {
  visit(*progAST);
  // Write all the data section last.
  x64.write("SECTION .data");
  for (uint32_t i = 0u; i < stringLiterals.size(); ++i) {
    const atl::shared_ptr<X64::StringLiteral> &strLiteral = stringLiterals[i];
    x64.string_literal(strLiteral->strName, strLiteral->strVal);
  }
  return outputSource;
}

/***** PRIVATE *****/

void GenerateX64::error(atl::string error) {
  errorCount++;
  errors.push_back(error);
}

/* ---- X64 Memory ---- */

void GenerateX64::alloc(const VarDecl &vd) {
  x64.write(vd.getIdentifier()->toString() + ": db " +
            atl::to_string(vd.getBytes()));
}

void GenerateX64::declareExternFuncs() {
  const atl::vector<atl::string> externFunDecls = {"malloc", "printf"};
  for (unsigned int idx = 0u; idx < externFunDecls.size(); ++idx) {
    x64.write("extern _" + externFunDecls[idx]);
  }
}

void GenerateX64::defOperator(const atl::string &op_name,
                              const atl::string &inst) {
  atl::stack<atl::shared_ptr<X64::Register>> paramRegs = x64.paramRegs();
  x64.block("FunDecl_" + op_name);
  x64.indent();
  x64.calleePrologue();

  x64.comment(" ---- Function Body ----");
  x64.mov(x64.rax, paramRegs.pop_back());
  x64.mov(x64.rcx, paramRegs.pop_back());
  x64.cmp(x64.rax, x64.rcx);
  x64.write(inst + " " + op_name + "_true");
  x64.jmp(op_name + "_false");

  x64.block(op_name + "_true");
  x64.mov(x64.rax, atl::shared_ptr<X64::IntValue>(new X64::IntValue(1)));
  x64.jmp(op_name + "_exit");
  x64.block(op_name + "_false");
  x64.mov(x64.rax, atl::shared_ptr<X64::IntValue>(new X64::IntValue(0)));
  x64.jmp(op_name + "_exit");
  x64.comment(" -----------------------");
  x64.block(op_name + "_exit");
  x64.calleeEpilogue();
  x64.ret();
  x64.unindent();
}

void GenerateX64::defSystemFunDecls() {
  atl::vector<atl::pair<atl::string, atl::string>> systemFunDecls = {
      {"FunDecl_malloc_int_", "_malloc"},
      {"FunDecl_printf_char_ptr__char_ptr_", "_printf"},
      {"FunDecl_printf_char_ptr__bool_", "_printf"},
      {"FunDecl_printf_char_ptr__char_", "_printf"},
      {"FunDecl_printf_char_ptr__int_", "_printf"},
      {"FunDecl_printf_char_ptr__unsigned_int_", "_printf"},
      {"FunDecl_printf_char_ptr_", "_printf"}};

  for (unsigned int i = 0u; i < systemFunDecls.size(); ++i) {
    const atl::pair<atl::string, atl::string> &systemFunDecl =
        systemFunDecls[i];
    x64.block(systemFunDecl.first);
    x64.indent();
    x64.mov(x64.rax, x64.rsp);
    x64.write("and rsp, -16");
    x64.push(x64.rax);
    x64.push(x64.rbp);
    x64.mov(x64.rbp, x64.rsp);
    x64.write("call " + systemFunDecl.second);
    x64.pop(x64.rbp);
    x64.pop(x64.rcx);
    x64.mov(x64.rsp, x64.rcx);
    x64.ret();
    x64.unindent();
  }

  // Define operator<
  defOperator("operator_lt", "jl");
  defOperator("operator_le", "jle");
  defOperator("operator_gt", "jg");
  defOperator("operator_ge", "jge");
  defOperator("operator_eq", "je");
  defOperator("operator_ne", "jne");
}

void GenerateX64::mainEntry() {
  x64.block("_main");
  x64.indent();
  // Save the state of the stack, and align it to 16 bytes.
  x64.mov(x64.rax, x64.rsp);
  x64.write("and rsp, -16");
  x64.push(x64.rax);
  x64.push(x64.rbp);
  x64.mov(x64.rbp, x64.rsp);
  // Call our main.
  x64.call("main_int__char_ptr_ptr_");
  // Restore state.
  x64.pop(x64.rbp);
  x64.pop(x64.rcx);
  x64.mov(x64.rsp, x64.rcx);
  x64.ret();
  x64.unindent();
}

void GenerateX64::mem_alloc(const atl::shared_ptr<X64::Operand> &num_bytes) {
  x64.callerPrologue();

  x64.mov(x64.rdi, num_bytes, "Number of bytes to allocate.");

  x64.write("call FunDecl_malloc_int_");

  x64.callerEpilogue();
}

/* ---- Visit AST ---- */

atl::shared_ptr<X64::Operand> GenerateX64::visit(AddressOf &ao) {
  const atl::shared_ptr<X64::Operand> aoObj = ao.addressOfExpr->accept(*this);
  x64.lea(x64.rax, aoObj, "Load Address of the Expression into RAX");
  return x64.rax;

  return atl::shared_ptr<X64::None>();
}
atl::shared_ptr<X64::Operand> GenerateX64::visit(Allocation &a) {
  // Allocate required bytes.
  const atl::shared_ptr<X64::Operand> numBytes = a.varType->accept(*this);
  mem_alloc(numBytes);
  return x64.rax;
}
atl::shared_ptr<X64::Operand> GenerateX64::visit(ArrayType &at) {
  return at.size->accept(*this);
}
atl::shared_ptr<X64::Operand> GenerateX64::visit(Assign &as) {
  const atl::shared_ptr<X64::Operand> rhs = as.rhs->accept(*this);

  atl::shared_ptr<X64::Register> rhsReg =
      copyToRegister(rhs, as.rhs->exprType->getBytes());
  x64.push(rhsReg, "Store RHS on the Stack Temporarily");

  const atl::shared_ptr<X64::Operand> lhs = as.lhs->accept(*this);
  x64.mov(x64.rcx, genIntValue(0));
  x64.pop(x64.rcx, "Pop the RHS off the Stack into rcx");
  // We can't mov a StringLiteral into a space on the stack
  // load it into a register(effectively the address) and
  // then move that address onto the stack.

  rhsReg = x64.getTempReg(as.rhs->exprType->getBytes(), 1);
  x64.mov(lhs, rhsReg, "Move RHS into LHS.");
  return atl::shared_ptr<X64::None>();
}
atl::shared_ptr<X64::Operand> GenerateX64::visit(BaseType &bt) {
  return atl::shared_ptr<X64::None>();
}
atl::shared_ptr<X64::Operand> GenerateX64::visit(BinOp &bo) {
  /* Get the value for the LHS. */
  atl::shared_ptr<X64::Operand> lhsOperand = bo.lhs->accept(*this);
  atl::shared_ptr<X64::Register> lhsReadReg =
      copyToRegister(lhsOperand, bo.lhs->exprType->getBytes());
  x64.push(lhsReadReg);

  /* Evaluate RHS and Store in EAX */
  atl::shared_ptr<X64::Operand> rhsOperand = bo.rhs->accept(*this);
  atl::shared_ptr<X64::Register> rhsReadReg =
      copyToRegister(rhsOperand, bo.rhs->exprType->getBytes());
  x64.mov(x64.rax, rhsReadReg, "Move RHS into RAX.");
  x64.pop(x64.rcx);

  /* Check if this Op is Overloaded */
  const atl::shared_ptr<FunDecl> opOverloadFun = bo.overload.lock();
  if (opOverloadFun) {
    atl::stack<atl::shared_ptr<X64::Register>> paramRegs = x64.paramRegs();
    x64.callerPrologue();
    x64.mov(paramRegs.pop_back(), x64.rcx);
    x64.mov(paramRegs.pop_back(), x64.rax);
    x64.call(opOverloadFun->getSignature().mangle(),
             "Call the Overloaded Function for this BinOp");
    x64.callerEpilogue();
    return x64.rax;
  }

  /* Perform Regular BinOp */
  switch (bo.operation) {
  case Op::ADD: {
    x64.add(x64.rax, x64.rcx);
    break;
  }
  case Op::SUB: {
    x64.sub(x64.rcx, x64.rax);
    x64.mov(x64.rax, x64.rcx);
    break;
  }
  case Op::MUL: {
    x64.imul(x64.rax, x64.rcx);
    break;
  }
  case Op::LT: {
    atl::stack<atl::shared_ptr<X64::Register>> paramRegs = x64.paramRegs();
    x64.callerPrologue();
    x64.mov(paramRegs.pop_back(), x64.rcx);
    x64.mov(paramRegs.pop_back(), x64.rax);
    x64.call("operator_lt");
    x64.callerEpilogue();
    break;
  }
  case Op::LE: {
    atl::stack<atl::shared_ptr<X64::Register>> paramRegs = x64.paramRegs();
    x64.callerPrologue();
    x64.mov(paramRegs.pop_back(), x64.rcx);
    x64.mov(paramRegs.pop_back(), x64.rax);
    x64.call("operator_le");
    x64.callerEpilogue();
    break;
  }
  case Op::GT: {
    atl::stack<atl::shared_ptr<X64::Register>> paramRegs = x64.paramRegs();
    x64.callerPrologue();
    x64.mov(paramRegs.pop_back(), x64.rcx);
    x64.mov(paramRegs.pop_back(), x64.rax);
    x64.call("operator_gt");
    x64.callerEpilogue();
    break;
  }
  case Op::GE: {
    atl::stack<atl::shared_ptr<X64::Register>> paramRegs = x64.paramRegs();
    x64.callerPrologue();
    x64.mov(paramRegs.pop_back(), x64.rcx);
    x64.mov(paramRegs.pop_back(), x64.rax);
    x64.call("operator_ge");
    x64.callerEpilogue();
    break;
  }
  case Op::EQ: {
    atl::stack<atl::shared_ptr<X64::Register>> paramRegs = x64.paramRegs();
    x64.callerPrologue();
    x64.mov(paramRegs.pop_back(), x64.rcx);
    x64.mov(paramRegs.pop_back(), x64.rax);
    x64.call("operator_eq");
    x64.callerEpilogue();
    break;
  }
  case Op::NE: {
    atl::stack<atl::shared_ptr<X64::Register>> paramRegs = x64.paramRegs();
    x64.callerPrologue();
    x64.mov(paramRegs.pop_back(), x64.rcx);
    x64.mov(paramRegs.pop_back(), x64.rax);
    x64.call("operator_ne");
    x64.callerEpilogue();
    break;
  }
  case Op::MOD: {
    // Swap rax and rcx
    x64.push(x64.rax);
    x64.mov(x64.rax, x64.rcx);
    x64.pop(x64.rcx);

    x64.mov(x64.rdx, atl::shared_ptr<X64::IntValue>(new X64::IntValue(0)));
    x64.idiv(x64.rcx);
    x64.mov(x64.rax, x64.rdx);
    break;
  }
  case Op::AND: {
    // TODO: Handle this earlier (we don't eval RHS if LHS is falsey)
    atl::stack<atl::shared_ptr<X64::Register>> paramRegs = x64.paramRegs();
    x64.callerPrologue();
    x64.mov(paramRegs.pop_back(), x64.rcx);
    x64.mov(paramRegs.pop_back(), x64.rax);
    x64.call("operator_eq");
    x64.callerEpilogue();

    paramRegs = x64.paramRegs();
    x64.callerPrologue();
    x64.mov(paramRegs.pop_back(), x64.rcx);
    x64.mov(paramRegs.pop_back(), genIntValue(0));
    x64.call("operator_ne");
    x64.callerEpilogue();
    break;
  }
  default:
    x64.comment("Not Implemented this BinOp Yet");
    break;
  }

  return x64.rax;
}
atl::shared_ptr<X64::Operand> GenerateX64::visit(Block &b) {
  b.scopeName = "Block" + atl::to_string(blockCount++);

  currScope = b.getptr();

  x64.comment(" --- " + currScope->scopeName + " --- ");
  x64.indent();
  for (unsigned int idx = 0; idx < b.stmts.size(); ++idx) {
    b.stmts[idx]->accept(*this);
  }
  x64.unindent();

  x64.block("Exit_" + currScope->scopeName);
  // TODO: Destruct Variables.
  for (int idx = currScope->objectsToDestruct.size() - 1; idx >= 0; --idx) {
    const atl::shared_ptr<VarDecl> currObj = currScope->objectsToDestruct[idx];
    x64.block("Exit_" + currScope->scopeName + "_" + atl::to_string(idx));
    x64.push(x64.rax);
    x64.callerPrologue();
    x64.lea(x64.rdi, addrOffset(x64.rbp, currObj->bpOffset),
            "Load the address of VarDecl '" + currObj->identifier->toString() +
                "'");
    x64.call("Dtor_" + currObj->type->mangle());
    x64.callerEpilogue();
    x64.pop(x64.rax);
  }

  currScope = currScope->outerScope.lock();

  x64.cmp(x64.r12, genIntValue(1));
  x64.je("Exit_" + currScope->scopeName);

  return atl::shared_ptr<X64::None>();
}
atl::shared_ptr<X64::Operand> GenerateX64::visit(BoolLiteral &bl) {
  if (bl.value == "true") {
    return genIntValue(1);
  } else {
    return genIntValue(0);
  }
}
atl::shared_ptr<X64::Operand> GenerateX64::visit(CharLiteral &cl) {
  const atl::string &charStr = cl.value;
  if (charStr[0] == '\\') {
    const char escapedChar = charStr[1];
    switch (escapedChar) {
    case '0':
      return genIntValue(static_cast<int>('\0'));
    case 'n':
      return genIntValue(static_cast<int>('\n'));
    default:
      error("Unsupported Char Literal");
      return atl::shared_ptr<X64::None>();
    }
  } else {
    const char charVal = charStr[0];
    return genIntValue(static_cast<int>(charVal));
  }
}
atl::shared_ptr<X64::Operand> GenerateX64::visit(ClassType &ct) {
  return atl::shared_ptr<X64::None>();
}
atl::shared_ptr<X64::Operand> GenerateX64::visit(ClassTypeDecl &ctd) {
  return atl::shared_ptr<X64::None>();
}
atl::shared_ptr<X64::Operand> GenerateX64::visit(ClassTypeDef &ctd) {
  // Tell the members their byte offset into the class.
  unsigned int objByteOffset = 0u;
  for (unsigned int idx = 0u; idx < ctd.classDecls.size(); ++idx) {
    atl::shared_ptr<Decl> currDecl = ctd.classDecls[idx];
    if (currDecl->astClass() != "VarDecl" && currDecl->astClass() != "VarDef") {
      continue;
    }
    atl::shared_ptr<VarDecl> currMember =
        atl::static_pointer_cast<VarDecl>(currDecl);
    currMember->bpOffset = objByteOffset;
    objByteOffset += currMember->getBytes();
  }

  // Generate code for methods.
  for (unsigned int idx = 0u; idx < ctd.classDecls.size(); ++idx) {
    const atl::shared_ptr<Decl> currDecl = ctd.classDecls[idx];
    if (currDecl->astClass() == "VarDecl" || currDecl->astClass() == "VarDef") {
      continue;
    }
    currDecl->accept(*this);
  }

  return atl::shared_ptr<X64::None>();
}
atl::shared_ptr<X64::Operand> GenerateX64::visit(ConstructorCall &cc) {
  atl::stack<atl::shared_ptr<X64::Register>> paramRegs = x64.paramRegs();

  x64.callerPrologue();

  // bpOffset is negative, so we add here.
  const atl::shared_ptr<VarDecl> objToCtruct = cc.objectToConstruct.lock();
  x64.lea(paramRegs.pop_back(), addrOffset(x64.rbp, objToCtruct->bpOffset),
          "Load the address of: " + objToCtruct->identifier->toString());

  const atl::shared_ptr<ConstructorDecl> ctorDecl = cc.constructorDecl.lock();
  for (uint argNum = 0; argNum < cc.constructorArgs.size(); ++argNum) {
    atl::shared_ptr<X64::Operand> argReg =
        cc.constructorArgs[argNum]->accept(*this);
    if (ctorDecl->constructorParams[argNum + 1]->type->astClass() ==
            "ReferenceType" &&
        cc.constructorArgs[argNum]->exprType->astClass() != "ReferenceType") {
      x64.lea(x64.rcx, argReg);
      argReg = x64.rcx;
    }
    if (paramRegs.size() > 0) {
      x64.mov(paramRegs.pop_back(), argReg);
    } else {
      x64.push(argReg);
    }
  }

  x64.call(cc.constructorDecl.lock()->getSignature().mangle());

  x64.callerEpilogue();
  return atl::shared_ptr<X64::None>(new X64::None());
}
atl::shared_ptr<X64::Operand> GenerateX64::visit(ConstructorDecl &cd) {
  return atl::shared_ptr<X64::None>();
}
atl::shared_ptr<X64::Operand> GenerateX64::visit(ConstructorDef &cd) {
  cd.scopeName = "FunDecl_" + cd.getSignature().mangle();
  if (cd.numCallers == 0) {
    return atl::shared_ptr<X64::None>();
  }

  currScope = cd.getptr();

  currBpOffset = 0;

  x64.block(currScope->scopeName);
  x64.indent();

  x64.calleePrologue();

  x64.comment(" ---- Constructor Args ----");
  x64.indent();
  atl::stack<atl::shared_ptr<X64::Register>> paramRegs = x64.paramRegs();
  for (unsigned int idx = 0; idx < cd.constructorParams.size(); ++idx) {
    const atl::shared_ptr<X64::Operand> argAddr =
        cd.constructorParams[idx]->accept(*this);
    x64.mov(argAddr, paramRegs.pop_back());
  }
  x64.unindent();
  x64.comment(" --------------------------");

  x64.comment(" ---- Constructor Initialiser List ----");
  x64.indent();
  for (unsigned int idx = 0; idx < cd.initialiserList.size(); ++idx) {
    cd.initialiserList[idx]->accept(*this);
  }
  x64.unindent();
  x64.comment(" --------------------------------------");

  x64.comment(" ---- Constructor Body ----");
  x64.indent();
  cd.constructorBlock->accept(*this);
  x64.unindent();
  x64.comment(" --------------------------");

  x64.block("Exit_" + currScope->scopeName);
  x64.indent();
  x64.calleeEpilogue();
  x64.ret();
  x64.unindent();
  x64.unindent();

  currBpOffset = 0;
  currScope = currScope->outerScope.lock();
  return atl::shared_ptr<X64::None>();
}
atl::shared_ptr<X64::Operand> GenerateX64::visit(Deletion &d) {
  return atl::shared_ptr<X64::None>();
}
atl::shared_ptr<X64::Operand> GenerateX64::visit(DestructorDecl &dd) {
  return atl::shared_ptr<X64::None>();
}
atl::shared_ptr<X64::Operand> GenerateX64::visit(DestructorDef &dd) {
  dd.scopeName = "Dtor_" + dd.classType->mangle();

  currScope = dd.getptr();

  currBpOffset = 0;

  x64.block("FunDecl_" + currScope->scopeName + "_const");
  x64.block("FunDecl_" + currScope->scopeName);
  x64.indent();

  x64.calleePrologue();

  x64.comment(" ---- Destructor Args ----");
  x64.indent();
  const atl::shared_ptr<X64::Operand> thisAddr = dd.thisParam->accept(*this);
  x64.mov(thisAddr, x64.rdi);
  x64.unindent();
  x64.comment(" -------------------------");

  x64.comment(" ---- Destructor Body ----");
  x64.indent();
  dd.destructorBlock->accept(*this);
  x64.unindent();
  x64.comment(" -------------------------");

  // TODO: Destruct members.

  x64.block("Exit_" + currScope->scopeName);
  x64.indent();
  x64.calleeEpilogue();
  x64.ret();
  x64.unindent();
  x64.unindent();

  currBpOffset = 0;
  currScope = currScope->outerScope.lock();
  return atl::shared_ptr<X64::None>();
}
atl::shared_ptr<X64::Operand> GenerateX64::visit(DoWhile &dw) {
  dw.body->accept(*this);
  dw.condition->accept(*this);
  return atl::shared_ptr<X64::None>();
}
atl::shared_ptr<X64::Operand> GenerateX64::visit(EnumClassTypeDecl &ectd) {
  return atl::shared_ptr<X64::None>();
}
atl::shared_ptr<X64::Operand> GenerateX64::visit(For &f) {
  return atl::shared_ptr<X64::None>();
}
atl::shared_ptr<X64::Operand> GenerateX64::visit(FunCall &fc) {
  atl::stack<atl::shared_ptr<X64::Register>> paramRegs = x64.paramRegs();

  x64.callerPrologue();

  const atl::shared_ptr<FunDecl> funDecl = fc.funDecl.lock();
  for (unsigned int argNum = 0u; argNum < fc.funArgs.size(); ++argNum) {
    atl::shared_ptr<X64::Operand> argReg = fc.funArgs[argNum]->accept(*this);
    if (funDecl->funParams[argNum]->type->astClass() == "ReferenceType") {
      x64.lea(x64.rcx, argReg);
      argReg = x64.rcx;
    }
    if (paramRegs.size() > 0)
      x64.mov(paramRegs.pop_back(), argReg);
    else
      x64.push(argReg);
  }
  x64.call(fc.funDecl.lock()->getSignature().mangle());

  x64.callerEpilogue();

  return x64.rax;
}
atl::shared_ptr<X64::Operand> GenerateX64::visit(FunDecl &fd) {
  return atl::shared_ptr<X64::None>();
}
atl::shared_ptr<X64::Operand> GenerateX64::visit(FunDef &fd) {
  fd.scopeName = "FunDecl_" + fd.getSignature().mangle();
  if (fd.numCallers == 0 && fd.funIdentifier->value != "main") {
    return atl::shared_ptr<X64::None>();
  }

  currScope = fd.getptr();

  currBpOffset = 0;

  x64.block(currScope->scopeName);
  x64.indent();

  x64.mov(x64.r12, genIntValue(0), "Returning Early Flag OFF");

  x64.calleePrologue();

  x64.comment(" ---- Function Args ----");
  x64.indent();
  atl::stack<atl::shared_ptr<X64::Register>> paramRegs = x64.paramRegs();
  for (unsigned int idx = 0; idx < fd.funParams.size(); ++idx) {
    const atl::shared_ptr<X64::Operand> argAddr =
        fd.funParams[idx]->accept(*this);
    x64.mov(argAddr, paramRegs.pop_back());
  }
  x64.unindent();
  x64.comment(" -----------------------");

  x64.comment(" ---- Function Body ----");
  x64.indent();
  fd.funBlock->accept(*this);
  x64.unindent();
  x64.comment(" -----------------------");

  x64.block("Exit_" + currScope->scopeName);
  x64.indent();
  x64.calleeEpilogue();
  x64.ret();
  x64.unindent();
  x64.unindent();

  currBpOffset = 0;
  currScope = currScope->outerScope.lock();
  return atl::shared_ptr<X64::None>();
}
atl::shared_ptr<X64::Operand> GenerateX64::visit(Identifier &i) {
  return atl::shared_ptr<X64::None>();
}
atl::shared_ptr<X64::Operand> GenerateX64::visit(If &i) {
  /* Calculate Names for Blocks */
  const atl::string trueBlockName =
      "ifTrueBlock" + atl::to_string(blockCount++);
  const atl::string falseBlockName =
      "ifFalseBlock" + atl::to_string(blockCount++);
  const atl::string endBlockName = "ifEndBlock" + atl::to_string(blockCount++);

  /* Calculate the result of the if condition. */
  atl::shared_ptr<X64::Operand> condResReg = i.ifCondition->accept(*this);
  x64.mov(x64.rax, condResReg, "if condition result in rax");

  /* Branch to False block if False, else branch to True block. */
  x64.cmp(x64.rax, genIntValue(0));
  x64.je(falseBlockName, "Condition == false");
  x64.jmp(trueBlockName, "Condition == true");

  /* Handle when the Case is True. */
  x64.block(trueBlockName, "Condition == true");
  i.ifBody->accept(*this);
  x64.jmp(endBlockName);

  /* Handle when the Case is False. */
  x64.block(falseBlockName, "Condition == false");
  if (i.elseBody)
    i.elseBody->accept(*this);
  x64.jmp(endBlockName);

  /* Handle after the If statement. */
  x64.block(endBlockName);
  return atl::shared_ptr<X64::None>();
}
atl::shared_ptr<X64::Operand> GenerateX64::visit(IntLiteral &il) {
  return atl::shared_ptr<X64::IntValue>(new X64::IntValue(il.getLiteral()));
}
atl::shared_ptr<X64::Operand> GenerateX64::visit(MemberAccess &ma) {
  const atl::shared_ptr<X64::Operand> objAddr = ma.object->accept(*this);
  const atl::shared_ptr<ClassTypeDef> objClassTypeDef = ma.objectTypeDef.lock();
  const atl::vector<atl::shared_ptr<Decl>> classDecls =
      objClassTypeDef->classDecls;
  atl::shared_ptr<VarDecl> memberDecl;
  for (unsigned int i = 0u; i < classDecls.size(); ++i) {
    const atl::shared_ptr<Decl> currDecl = classDecls[i];
    if (currDecl->astClass() != "VarDecl" && currDecl->astClass() != "VarDef") {
      continue;
    }
    const atl::shared_ptr<VarDecl> currMember =
        atl::static_pointer_cast<VarDecl>(currDecl);
    if (currMember == ma.fieldVariable->varDecl.lock()) {
      memberDecl = currMember;
      break;
    }
  }
  const unsigned int objByteOffset = memberDecl->bpOffset;
  // Handle Pointer Access
  if (ma.accessType == SourceToken::Class::PTRDOT ||
      ma.object->exprType->astClass() == "ReferenceType") {
    x64.mov(x64.rax, objAddr, "Move objects address into rax");
    return addrOffset(x64.rax, objByteOffset);
  } else {
    return addrOffset(objAddr, objByteOffset);
  }
}
atl::shared_ptr<X64::Operand> GenerateX64::visit(MemberCall &mc) {
  atl::stack<atl::shared_ptr<X64::Register>> paramRegs = x64.paramRegs();

  x64.callerPrologue();

  /* Handle `this` parameter. */
  const atl::shared_ptr<Expr> thisPtrExpr =
      (mc.object->exprType->astClass() == "ReferenceType")
          ? mc.object
          : atl::shared_ptr<Expr>(new AddressOf(mc.object));
  const atl::shared_ptr<X64::Operand> this_ptr = thisPtrExpr->accept(*this);
  x64.mov(paramRegs.pop_back(), this_ptr);

  const FunCall &fc = *mc.funCall;
  for (unsigned int argNum = 0u; argNum < fc.funArgs.size(); ++argNum) {
    const atl::shared_ptr<X64::Operand> argReg =
        fc.funArgs[argNum]->accept(*this);
    if (paramRegs.size() > 0) {
      x64.mov(paramRegs.pop_back(), argReg);
    } else {
      x64.push(argReg);
    }
  }

  x64.call(fc.funDecl.lock()->getSignature().mangle());

  x64.callerEpilogue();

  return x64.rax;
}
atl::shared_ptr<X64::Operand> GenerateX64::visit(Namespace &n) {
  for (unsigned int idx = 0; idx < n.namespaceDecls.size(); ++idx)
    n.namespaceDecls[idx]->accept(*this);

  return nullptr;
}
atl::shared_ptr<X64::Operand> GenerateX64::visit(Not &n) {
  /* Calculate Names for Blocks */
  const atl::string trueBlockName =
      "ifTrueBlock" + atl::to_string(blockCount++);
  const atl::string falseBlockName =
      "ifFalseBlock" + atl::to_string(blockCount++);
  const atl::string endBlockName = "ifEndBlock" + atl::to_string(blockCount++);

  /* Calculate the boolean value of the not expression. */
  const atl::shared_ptr<X64::Operand> exprRes = n.expr->accept(*this);
  x64.mov(x64.rax, exprRes);

  /* Branch to False block if False, else branch to True block. */
  x64.cmp(x64.rax, atl::shared_ptr<X64::IntValue>(new X64::IntValue(0)));
  x64.je(falseBlockName);
  x64.jmp(trueBlockName);

  /* Handle when the Case is True. */
  x64.block(trueBlockName);
  x64.mov(x64.rax, atl::shared_ptr<X64::IntValue>(new X64::IntValue(0)));
  x64.jmp(endBlockName);

  /* Handle when the Case is False. */
  x64.block(falseBlockName);
  x64.mov(x64.rax, atl::shared_ptr<X64::IntValue>(new X64::IntValue(1)));
  x64.jmp(endBlockName);

  /* Handle after the If statement. */
  x64.block(endBlockName);

  return x64.rax;
}
atl::shared_ptr<X64::Operand> GenerateX64::visit(Nullptr &n) {
  return genIntValue(0);
}
atl::shared_ptr<X64::Operand> GenerateX64::visit(ParenthExpr &pe) {
  return pe.innerExpr->accept(*this);
}
atl::shared_ptr<X64::Operand> GenerateX64::visit(PointerType &pt) {
  return atl::shared_ptr<X64::None>();
}
atl::shared_ptr<X64::Operand> GenerateX64::visit(PrefixOp &po) {
  /* Get the value for the prefix-ops expression. */
  atl::shared_ptr<X64::Operand> varLoc = po.variable->accept(*this);
  x64.mov(x64.rax, varLoc, "Copy var value into rax");
  switch (po.operation) {
  case PrefixOp::Op::INC: {
    x64.add(x64.rax, genIntValue(1), "Update Value");
    break;
  }
  case PrefixOp::Op::DEC: {
    x64.sub(x64.rax, genIntValue(1), "Update Value");
    break;
  }
  default:
    x64.comment("Not Implemented this PrefixOp Yet");
    break;
  }

  x64.mov(varLoc, x64.rax, "Write back to var.");

  return atl::shared_ptr<X64::None>();
}
atl::shared_ptr<X64::Operand> GenerateX64::visit(Program &p) {
  currScope = p.getptr();

  /* Create Global Variables */
  x64.write("SECTION .data");
  for (unsigned int idx = 0; idx < p.globalVars.size(); ++idx) {
    this->alloc(*p.globalVars[idx]);
  }

  /* Declare External Functions */
  x64.write("SECTION .text");
  x64.write("global _main");
  declareExternFuncs();

  /* Set up this file's entry point. */
  mainEntry();

  /* Forward Declare known system functions. */
  defSystemFunDecls();

  /* Generate Code for this AST */
  for (unsigned int idx = 0u; idx < p.decls.size(); ++idx) {
    const atl::shared_ptr<Decl> currDecl = p.decls[idx];
    // Skip global variables as we have handled them already.
    if (currDecl->astClass() == "VarDecl" || currDecl->astClass() == "VarDef") {
      continue;
    }
    x64.write("\n\n");
    currDecl->accept(*this);
    x64.write("\n\n");
  }

  return atl::shared_ptr<X64::None>();
}
atl::shared_ptr<X64::Operand> GenerateX64::visit(ReferenceType &rt) {
  return atl::shared_ptr<X64::None>();
}
atl::shared_ptr<X64::Operand> GenerateX64::visit(Return &r) {
  if (r.returnExpr) {
    atl::shared_ptr<X64::Operand> rVal = r.returnExpr->accept(*this);
    x64.mov(x64.rax, rVal);
  }

  x64.mov(x64.r12, genIntValue(1), "Returning Early Flag ON");
  x64.jmp("Exit_" + currScope->scopeName);

  return atl::shared_ptr<X64::None>();
}
atl::shared_ptr<X64::Operand> GenerateX64::visit(SizeOf &so) {
  return atl::shared_ptr<X64::None>();
}
atl::shared_ptr<X64::Operand> GenerateX64::visit(StaticCast &sc) {
  return sc.expr->accept(*this);
}
atl::shared_ptr<X64::Operand> GenerateX64::visit(StringLiteral &sl) {
  const atl::string strName = "strLiteral" + atl::to_string(stringCount++);
  const atl::string strVal = sl.value;
  const atl::shared_ptr<X64::StringLiteral> strLit(
      new X64::StringLiteral(strName, strVal));
  stringLiterals.push_back(strLit);
  return strLit;
}
atl::shared_ptr<X64::Operand> GenerateX64::visit(SubscriptOp &so) {
  if (!so.operatorDecl.expired()) {
    atl::stack<atl::shared_ptr<X64::Register>> paramRegs = x64.paramRegs();

    x64.callerPrologue();

    /* Handle `this` parameter. */
    const atl::shared_ptr<X64::Operand> this_ptr = so.variable->accept(*this);
    x64.mov(paramRegs.pop_back(), this_ptr);

    const atl::shared_ptr<X64::Operand> index = so.index->accept(*this);
    x64.mov(paramRegs.pop_back(), index);

    x64.call(so.operatorDecl.lock()->getSignature().mangle());

    x64.callerEpilogue();

    return x64.rax;
  } else {
    // Get a pointer to the variable.
    const atl::shared_ptr<X64::Operand> this_ptr = so.variable->accept(*this);
    x64.mov(x64.rax, this_ptr, "Pointer to subscript variable.");
    x64.push(x64.rax, "Save ptr to subscript var to stack.");

    // Get the index to offset.
    const atl::shared_ptr<X64::Operand> index = so.index->accept(*this);
    const atl::shared_ptr<X64::Register> indexReg =
        copyToRegister(index, so.index->exprType->getBytes());
    x64.pop(x64.rcx, "Restore ptr to subscript var from stack.");
    x64.add(x64.rcx, indexReg, "Increment the ptr by appropriate amount.");
    x64.mov(x64.rax, x64.rcx, "Move the new address into RAX");

    return addrOffset(x64.rax, 0);
  }

  return atl::shared_ptr<X64::None>();
}
atl::shared_ptr<X64::Operand> GenerateX64::visit(TemplateDef &td) {
  return atl::shared_ptr<X64::None>();
}
atl::shared_ptr<X64::Operand> GenerateX64::visit(TertiaryExpr &t) {
  return atl::shared_ptr<X64::None>();
}
atl::shared_ptr<X64::Operand> GenerateX64::visit(Throw &t) {
  return atl::shared_ptr<X64::None>();
}
atl::shared_ptr<X64::Operand> GenerateX64::visit(TypeDefDecl &td) {
  return atl::shared_ptr<X64::None>();
}
atl::shared_ptr<X64::Operand> GenerateX64::visit(ValueAt &va) {
  const atl::shared_ptr<Type> vaType = va.exprType;
  if (vaType->astClass() == "BaseType") {
    const atl::shared_ptr<X64::Operand> exprOperand =
        va.derefExpr->accept(*this);

    x64.mov(x64.rax, exprOperand, "Move address into rax");
    return addrOffset(x64.rax, 0);
  } else {
    printf("Dereferencing Non-Primitive Types not Supported Yet.\n");
    throw;
  }
}
atl::shared_ptr<X64::Operand> GenerateX64::visit(VarDecl &vd) {
  const int bytesRequired = roundTo16Bytes(vd.getBytes());
  currBpOffset -= bytesRequired;
  vd.bpOffset = currBpOffset;

  const atl::string comment =
      "Allocated " + atl::to_string(vd.getBytes()) +
      "B (16B Aligned) for VarDecl: " + vd.getIdentifier()->toString() +
      " @ [rbp" + atl::to_string(currBpOffset) + "]";

  x64.sub(x64.rsp,
          atl::shared_ptr<X64::IntValue>(new X64::IntValue(bytesRequired)),
          comment);

  if (vd.type->astClass() == "ClassType") {
    currScope->objectsToDestruct.push_back(vd.getptr());
  }

  return atl::shared_ptr<X64::AddrOffset>(
      new X64::AddrOffset(x64.rbp, vd.bpOffset));
}
atl::shared_ptr<X64::Operand> GenerateX64::visit(VarDef &vd) {
  const atl::string vdIdent = vd.getIdentifier()->toString();
  const int bytesRequired = roundTo16Bytes(vd.getBytes());
  currBpOffset -= bytesRequired;
  vd.bpOffset = currBpOffset;

  const atl::string comment = "Allocated " + atl::to_string(vd.getBytes()) +
                              "B (16B Aligned) for VarDef: " + vdIdent +
                              " @ [rbp" + atl::to_string(currBpOffset) + "]";

  x64.sub(x64.rsp,
          atl::shared_ptr<X64::IntValue>(new X64::IntValue(bytesRequired)),
          comment);

  const atl::shared_ptr<X64::Operand> valueOperand = vd.varValue->accept(*this);

  if (valueOperand->opType() != "None") {
    x64.mov(x64.rax, valueOperand,
            "Move " + vdIdent + "'s value into temp register.");
    x64.mov(addrOffset(x64.rbp, vd.bpOffset), x64.rax,
            "Move " + vdIdent + "'s value into its stack allocated space.");
  }

  if (vd.type->astClass() == "ClassType") {
    currScope->objectsToDestruct.push_back(vd.getptr());
  }

  return atl::shared_ptr<X64::AddrOffset>(
      new X64::AddrOffset(x64.rbp, vd.bpOffset));
}
atl::shared_ptr<X64::Operand> GenerateX64::visit(VarExpr &ve) {
  /* Find this Variable's Location in the Stack, and Load It. */
  const atl::shared_ptr<VarDecl> varDecl = ve.varDecl.lock();
  if (varDecl->isGlobal) {
    return atl::shared_ptr<X64::GlobalVariable>(
        new X64::GlobalVariable(ve.varDecl.lock()->getIdentifier()->toString(),
                                ve.varDecl.lock()->getBytes()));
  }

  return addrOffset(x64.rbp, varDecl->bpOffset);

  // atl::shared_ptr<X64::Operand> varAddr = addrOffset(x64.rbp,
  // varDecl->bpOffset); atl::shared_ptr<Type> varType = varDecl->type; while
  // (varType->astClass() == "PointerType") {
  //   x64.mov(x64.rax, varAddr);
  //   varAddr = x64.rax;
  //   varType = atl::static_pointer_cast<PointerType>(varType)->pointedType;
  // }

  // return varAddr;
}
atl::shared_ptr<X64::Operand> GenerateX64::visit(While &w) {
  const atl::string whileCondition = "whileCond" + atl::to_string(blockCount++);
  const atl::string whileStart = "whileStart" + atl::to_string(blockCount++);
  const atl::string whileEnd = "whileEnd" + atl::to_string(blockCount++);

  /* Create Condition Block */
  x64.block(whileCondition);
  const atl::shared_ptr<X64::Operand> condRes = w.condition->accept(*this);
  if (condRes->opType() != "Register") {
    const atl::shared_ptr<X64::Register> condReg =
        x64.getTempReg(w.condition->exprType->getBytes(), 1);
    x64.mov(x64.rcx, genIntValue(0), "Set rcx to all 0");
    x64.mov(condReg, condRes,
            "Move the condition result into its size specific register.");
  } else {
    x64.mov(x64.rcx, condRes, "Move the LHS into RCX");
  }
  x64.mov(x64.rax, x64.rcx);

  x64.mov(x64.rcx, atl::shared_ptr<X64::IntValue>(new X64::IntValue(1)));
  x64.cmp(x64.rax, x64.rcx);
  x64.write("je " + whileStart);
  x64.jmp(whileEnd);

  x64.block(whileStart);
  w.body->accept(*this);
  x64.jmp(whileCondition);

  x64.block(whileEnd);
  return atl::shared_ptr<X64::None>();
}

/* ---- Helpers ---- */

atl::shared_ptr<X64::IntValue> GenerateX64::genIntValue(int value) const {
  return atl::shared_ptr<X64::IntValue>(
      new X64::IntValue(atl::to_string(value)));
}

atl::shared_ptr<X64::AddrOffset>
GenerateX64::addrOffset(const atl::shared_ptr<X64::Operand> addrOperand,
                        const int offset) {
  return atl::shared_ptr<X64::AddrOffset>(
      new X64::AddrOffset(addrOperand, offset));
}

int GenerateX64::roundTo16Bytes(int bytes) const {
  while (bytes % 16 != 0)
    ++bytes;
  return bytes;
}

atl::shared_ptr<X64::Register>
GenerateX64::copyToRegister(const atl::shared_ptr<X64::Operand> &operand,
                            const unsigned int num_bytes) {
  if (operand->opType() == "Register") {
    return operand;
  }

  x64.push(x64.rcx);

  const atl::shared_ptr<X64::Register> reg = x64.getTempReg(num_bytes, 1);
  x64.mov(x64.rcx, genIntValue(0), "Set rcx to all 0");
  x64.mov(reg, operand, "Move operand into its size specific register.");
  x64.mov(x64.rax, x64.rcx);

  x64.pop(x64.rcx);

  return x64.rax;
}