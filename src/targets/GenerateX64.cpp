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
}

void GenerateX64::malloc(const atl::shared_ptr<X64::Operand> &num_bytes) {
  x64.callerPrologue();

  x64.mov(x64.rdi, num_bytes);

  x64.write("call FunDecl_malloc_int_");

  x64.callerEpilogue();
}

/* ---- Visit AST ---- */

atl::shared_ptr<X64::Operand> GenerateX64::visit(AddressOf &ao) {
  const atl::shared_ptr<X64::Operand> aoObj = ao.addressOfExpr->accept(*this);
  if (aoObj->opType() == "AddrOffset") {
    // Calculate the address, and store it in
    const atl::shared_ptr<X64::AddrOffset> addrOffset =
        atl::static_pointer_cast<X64::AddrOffset>(aoObj);

    x64.mov(x64.rax, addrOffset->addrOperand);
    const atl::shared_ptr<X64::IntValue> offset(
        new X64::IntValue(addrOffset->offset));
    x64.add(x64.rax, offset);

    return x64.rax;
  }

  return atl::shared_ptr<X64::None>();
}
atl::shared_ptr<X64::Operand> GenerateX64::visit(Allocation &a) {
  if (a.varType != nullptr) {
    // Allocate required bytes.
    const atl::shared_ptr<X64::Operand> numBytes = a.varType->accept(*this);
    malloc(numBytes);
    return x64.rax;
  } else {
    // Handle allocating structs.
    return x64.rax;
  }
}
atl::shared_ptr<X64::Operand> GenerateX64::visit(ArrayType &at) {
  return at.size->accept(*this);
}
atl::shared_ptr<X64::Operand> GenerateX64::visit(Assign &as) {
  const atl::shared_ptr<X64::Operand> rhs = as.rhs->accept(*this);
  x64.mov(x64.rax, rhs, "Move the RHS into a temp register");
  x64.push(x64.rax, "Store RHS on the Stack Temporarily");

  const atl::shared_ptr<X64::Operand> lhs = as.lhs->accept(*this);
  x64.pop(x64.rcx, "Pop the RHS off the Stack into rcx");
  // We can't mov a StringLiteral into a space on the stack
  // load it into a register(effectively the address) and
  // then move that address onto the stack.

  x64.mov(lhs, x64.rcx, "Move RHS into LHS.");
  return atl::shared_ptr<X64::None>();
}
atl::shared_ptr<X64::Operand> GenerateX64::visit(BaseType &bt) {
  return atl::shared_ptr<X64::None>();
}
atl::shared_ptr<X64::Operand> GenerateX64::visit(BinOp &bo) {
  /* Get the value for the LHS. */
  atl::shared_ptr<X64::Operand> lhsOperand = bo.lhs->accept(*this);
  if (lhsOperand->opType() == "Register") {
    x64.push(lhsOperand, "Store LHS to Stack");
  } else {
    const atl::shared_ptr<X64::Register> tempReg = x64.getTempReg();
    x64.mov(tempReg, lhsOperand, "Move the LHS into temp register.");
    x64.push(tempReg, "Push temp register onto the stack.");
  }

  /* Evaluate RHS and Store in EAX */
  atl::shared_ptr<X64::Operand> rhsOperand = bo.rhs->accept(*this);
  x64.pop(x64.rcx, "Restore LHS from Stack into RCX.");
  x64.mov(x64.rax, rhsOperand, "Move RHS into RCX.");

  /* Check if this Op is Overloaded */
  const atl::shared_ptr<FunDecl> opOverloadFun = bo.overload.lock();
  if (opOverloadFun) {
    atl::stack<atl::shared_ptr<X64::Register>> paramRegs = x64.paramRegs();
    x64.callerPrologue();
    x64.mov(paramRegs.pop_back(), x64.rcx);
    x64.mov(paramRegs.pop_back(), x64.rax);
    x64.call(opOverloadFun->getSignature().mangle());
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
  }
  default:
    x64.comment("Not Implemented this BinOp Yet");
    break;
  }

  return x64.rax;
}
atl::shared_ptr<X64::Operand> GenerateX64::visit(Block &b) {
  currScope = b.getptr();
  for (unsigned int idx = 0; idx < b.stmts.size(); ++idx)
    b.stmts[idx]->accept(*this);

  currScope = b.outerScope.lock();
  return atl::shared_ptr<X64::None>();
}
atl::shared_ptr<X64::Operand> GenerateX64::visit(BoolLiteral &bl) {
  if (bl.value == "true") {
    return atl::shared_ptr<X64::IntValue>(
        new X64::IntValue(atl::string(1, '1')));
  } else {
    return atl::shared_ptr<X64::IntValue>(
        new X64::IntValue(atl::string(1, '0')));
  }
}
atl::shared_ptr<X64::Operand> GenerateX64::visit(CharLiteral &cl) {
  const atl::string &charStr = cl.value;
  if (charStr[0] == '\\') {
    const char escapedChar = charStr[1];
    switch (escapedChar) {
    case '0':
      return atl::shared_ptr<X64::IntValue>(
          new X64::IntValue(static_cast<int>('\0')));
    case 'n':
      return atl::shared_ptr<X64::IntValue>(
          new X64::IntValue(static_cast<int>('\n')));
    default:
      error("Unsupported Char Literal");
      return atl::shared_ptr<X64::None>();
    }
  } else {
    const char charVal = charStr[0];
    return atl::shared_ptr<X64::IntValue>(
        new X64::IntValue(static_cast<int>(charVal)));
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
    // TODO: MASSIVE HACK, this is just so that each var gets its own line,
    // seems inefficient...
    objByteOffset += 16; // currMember->getBytes();
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

  x64.mov(x64.rax, x64.rbp);

  // bpOffset is negative, so we add here.
  x64.add(x64.rax, genIntValue(cc.objectToConstruct.lock()->bpOffset));
  x64.mov(paramRegs.pop_back(), x64.rax);

  for (uint argNum = 0; argNum < cc.constructorArgs.size(); ++argNum) {
    const atl::shared_ptr<X64::Operand> argReg =
        cc.constructorArgs[argNum]->accept(*this);
    if (paramRegs.size() > 0) {
      x64.mov(paramRegs.pop_back(), argReg);
    } else {
      x64.push(argReg);
    }
  }

  x64.call(cc.constructorDecl.lock()->getSignature().mangle());
  return atl::shared_ptr<X64::None>(new X64::None());
}
atl::shared_ptr<X64::Operand> GenerateX64::visit(ConstructorDecl &cd) {
  return atl::shared_ptr<X64::None>();
}
atl::shared_ptr<X64::Operand> GenerateX64::visit(ConstructorDef &cd) {
  if (cd.numCallers == 0) {
    return atl::shared_ptr<X64::None>();
  }

  atl::stack<atl::shared_ptr<X64::Register>> paramRegs = x64.paramRegs();

  currScope = cd.constructorBlock;
  currBpOffset = 0;

  x64.block("FunDecl_" + cd.getSignature().mangle());

  x64.calleePrologue();

  x64.comment(" ---- Constructor Args ----");
  for (unsigned int idx = 0; idx < cd.constructorParams.size(); ++idx) {
    const atl::shared_ptr<X64::Operand> argAddr =
        cd.constructorParams[idx]->accept(*this);
    x64.mov(argAddr, paramRegs.pop_back());
  }
  x64.comment(" --------------------------");

  x64.comment(" ---- Constructor Initialiser List ----");
  for (unsigned int idx = 0; idx < cd.initialiserList.size(); ++idx) {
    cd.initialiserList[idx]->accept(*this);
  }
  x64.comment(" --------------------------------------");

  x64.comment(" ---- Constructor Body ----");
  cd.constructorBlock->accept(*this);
  x64.comment(" --------------------------");

  x64.calleeEpilogue();

  currBpOffset = 0;
  currScope = cd.constructorBlock->outerScope.lock();
  return atl::shared_ptr<X64::None>();
}
atl::shared_ptr<X64::Operand> GenerateX64::visit(Deletion &d) {
  return atl::shared_ptr<X64::None>();
}
atl::shared_ptr<X64::Operand> GenerateX64::visit(DestructorDecl &dd) {
  return atl::shared_ptr<X64::None>();
}
atl::shared_ptr<X64::Operand> GenerateX64::visit(DestructorDef &dd) {
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

  for (unsigned int argNum = 0u; argNum < fc.funArgs.size(); ++argNum) {
    const atl::shared_ptr<X64::Operand> argReg =
        fc.funArgs[argNum]->accept(*this);
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
  if (fd.numCallers == 0 && fd.funIdentifier->value != "main") {
    return atl::shared_ptr<X64::None>();
  }
  atl::stack<atl::shared_ptr<X64::Register>> paramRegs = x64.paramRegs();

  currScope = fd.funBlock;

  x64.block("FunDecl_" + fd.getSignature().mangle());

  x64.calleePrologue();

  x64.comment(" ---- Function Args ----");
  for (unsigned int idx = 0; idx < fd.funParams.size(); ++idx) {
    const atl::shared_ptr<X64::Operand> argAddr =
        fd.funParams[idx]->accept(*this);
    x64.mov(argAddr, paramRegs.pop_back());
  }
  x64.comment(" -----------------------");

  x64.comment(" ---- Function Body ----");
  fd.funBlock->accept(*this);
  x64.comment(" -----------------------");

  x64.calleeEpilogue();

  currBpOffset = 0;
  currScope = fd.funBlock->outerScope.lock();
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

  /* Branch to False block if False, else branch to True block. */
  x64.cmp(condResReg, atl::shared_ptr<X64::IntValue>(new X64::IntValue(0)));
  x64.je(falseBlockName);
  x64.jmp(trueBlockName);

  /* Handle when the Case is True. */
  x64.block(trueBlockName);
  i.ifBody->accept(*this);
  x64.jmp(endBlockName);

  /* Handle when the Case is False. */
  x64.block(falseBlockName);
  if (i.elseBody)
    i.elseBody->accept(*this);
  x64.jmp(endBlockName);

  /* Handle after the If statement. */
  x64.block(endBlockName);
  return atl::shared_ptr<X64::None>();
}
atl::shared_ptr<X64::Operand> GenerateX64::visit(IntLiteral &il) {
  x64.mov(x64.rax,
          atl::shared_ptr<X64::IntValue>(new X64::IntValue(il.getLiteral())));
  return x64.rax;
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
  if (ma.accessType == SourceToken::Class::PTRDOT) {
    x64.mov(x64.rax, objAddr);
    return addrOffset(x64.rax, objByteOffset);
  } else {
    return addrOffset(objAddr, objByteOffset);
  }
}
atl::shared_ptr<X64::Operand> GenerateX64::visit(MemberCall &mc) {
  atl::stack<atl::shared_ptr<X64::Register>> paramRegs = x64.paramRegs();

  x64.callerPrologue();

  /* Handle `this` parameter. */
  const atl::shared_ptr<AddressOf> thisPtrExpr(new AddressOf(mc.object));
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
  atl::shared_ptr<X64::Operand> varReg = po.variable->accept(*this);
  x64.mov(x64.rax, varReg);
  switch (po.operation) {
  case PrefixOp::Op::INC: {
    x64.add(x64.rax, atl::shared_ptr<X64::IntValue>(new X64::IntValue(1)));
    break;
  }
  case PrefixOp::Op::DEC: {
    x64.sub(x64.rax, atl::shared_ptr<X64::IntValue>(new X64::IntValue(1)));
    break;
  }
  default:
    x64.comment("Not Implemented this PrefixOp Yet");
    break;
  }

  x64.mov(po.variable->accept(*this), x64.rax);

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
  x64.calleeEpilogue();
  x64.ret();
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
    x64.push(x64.rax);

    // Get the index to offset.
    const atl::shared_ptr<X64::Operand> index = so.index->accept(*this);
    x64.pop(x64.rcx);
    x64.add(x64.rcx, index);
    x64.mov(x64.rax, x64.rcx);
    const atl::shared_ptr<X64::AddrOffset> valueAtIndex(
        new X64::AddrOffset(x64.rax, 0));
    return valueAtIndex;
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

    const atl::shared_ptr<BaseType> valueType =
        atl::static_pointer_cast<BaseType>(vaType);
    const atl::shared_ptr<X64::Register> destReg =
        getRegisterForType(valueType->primitiveType);
    x64.mov(x64.rcx, exprOperand, "Move address into rcx");
    x64.mov(x64.rax, genIntValue(0), "Set rax to all 0");
    x64.mov(destReg, addrOffset(x64.rcx, 0));
    // x64.mov(destReg, x64.rcx);
    return x64.rax;
    // return atl::shared_ptr<X64::AddrOffset>(new X64::AddrOffset(x64.rax, 0));
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
  return atl::shared_ptr<X64::AddrOffset>(
      new X64::AddrOffset(x64.rbp, vd.bpOffset));
  // return atl::shared_ptr<X64::None>();
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
  x64.mov(x64.rax, condRes);
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

atl::shared_ptr<X64::IntValue> GenerateX64::genIntValue(int value) {
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
GenerateX64::getRegisterForType(const PrimitiveType &type) const {
  switch (type) {
  case PrimitiveType::CHAR:
    return x64.al;
  case PrimitiveType::BOOL:
    return x64.al;
  case PrimitiveType::INT:
    return x64.eax;
  case PrimitiveType::NULLPTR_T:
    return x64.rax;
  case PrimitiveType::SHORT:
    return x64.ax;
  case PrimitiveType::UINT:
    return x64.eax;
  case PrimitiveType::VOID:
    return x64.rax;
  }
  printf("FATAL ERROR: Unknown PrimitiveType\n");
  throw;
}
