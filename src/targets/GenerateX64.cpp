#include "targets/GenerateX64.h"

using namespace ACC;

GenerateX64::GenerateX64(atl::shared_ptr<Program> progAST,
                         const atl::string &outputFile)
    : progAST(progAST), x64(outputFile) {
}

void GenerateX64::printErrors() const {
  printf("FATAL x64 Generation Errors:\n");
  for (unsigned int idx = 0; idx < errors.size(); ++idx)
    printf("\t%s\n", errors[idx].c_str());
}

void GenerateX64::run() { visit(*progAST); }


/***** PRIVATE *****/

void GenerateX64::error(atl::string error) {
  errorCount++;
  errors.push_back(error);
}


  /* ---- X64 Memory ---- */

void GenerateX64::alloc(const VarDecl &vd) {
  x64.write(vd.getIdentifier()->toString() + ": db " + atl::to_string(vd.getBytes()));
}

void GenerateX64::declareExternFuncs() {
  const atl::vector<atl::string> externFunDecls = { "printf" };
  for (unsigned int idx = 0u; idx < externFunDecls.size(); ++idx) {
    x64.write("extern _" + externFunDecls[idx]);
  }
}

void GenerateX64::defSystemFunDecls() {
  atl::vector<atl::pair<atl::string, atl::string>> systemFunDecls = {
    { "FunDecl_printf_char_ptr__char_ptr_", "_printf"},
    { "FunDecl_printf_char_ptr__int_", "_printf" }
  };

  for (unsigned int i = 0u; i < systemFunDecls.size(); ++i) {
    const atl::pair<atl::string, atl::string> &systemFunDecl = systemFunDecls[i];
    x64.block(systemFunDecl.first);
    x64.write("call " + systemFunDecl.second);
    x64.ret();
  }
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
  x64.pop(x64.rax);
  x64.mov(x64.rsp, x64.rax);
  x64.ret();
}

/* ---- Visit AST ---- */

atl::shared_ptr<X64::Operand> GenerateX64::visit(AddressOf &ao) {
  return atl::shared_ptr<X64::None>();
}
atl::shared_ptr<X64::Operand> GenerateX64::visit(Allocation &a) {
  return atl::shared_ptr<X64::None>();
}
atl::shared_ptr<X64::Operand> GenerateX64::visit(ArrayType &at) {
  return atl::shared_ptr<X64::None>();
}
atl::shared_ptr<X64::Operand> GenerateX64::visit(Assign &as) {
  atl::shared_ptr<X64::Operand> lhsRegRes = as.lhs->accept(*this);
  atl::shared_ptr<X64::Operand> rhsResReg = as.rhs->accept(*this);
  // We can't mov a StringLiteral into a space on the stack
  // load it into a register(effectively the address) and 
  // then move that address onto the stack.
  const atl::shared_ptr<X64::Register> tempReg = x64.getTempReg();
  x64.mov(tempReg, rhsResReg, "Move RHS into a register.");
  x64.mov(lhsRegRes, tempReg, "Move register into LHS.");
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

  /* Perform BinOp */
  switch (bo.operation) {
  case Op::ADD: {
    x64.add(x64.rax, x64.rcx);
    break;
  }
  case Op::MUL: {
    x64.imul(x64.rax, x64.rcx);
    break;
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
  return atl::shared_ptr<X64::None>();
}
atl::shared_ptr<X64::Operand> GenerateX64::visit(CharLiteral &cl) {
  return atl::shared_ptr<X64::None>();
}
atl::shared_ptr<X64::Operand> GenerateX64::visit(ClassType &ct) {
  return atl::shared_ptr<X64::None>();
}
atl::shared_ptr<X64::Operand> GenerateX64::visit(ClassTypeDecl &ctd) {
  return atl::shared_ptr<X64::None>();
}
atl::shared_ptr<X64::Operand> GenerateX64::visit(ClassTypeDef &ctd) {
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

  const atl::shared_ptr<X64::AddrOffset> objectAddr(new X64::AddrOffset(x64.rsp, cc.objectToConstruct.lock()->bpOffset));
  x64.mov(paramRegs.pop_back(), objectAddr);

  for (uint argNum = 0; argNum < cc.constructorArgs.size(); ++argNum) {
    const atl::shared_ptr<X64::Operand> argReg = cc.constructorArgs[argNum]->accept(*this);
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
    const atl::shared_ptr<X64::Operand> argAddr = cd.constructorParams[idx]->accept(*this);
    x64.mov(argAddr, paramRegs.pop_back());
  }
  x64.comment(" --------------------------");


  x64.comment(" ---- Constructor Initialiser List ----");
  for (unsigned int idx = 0; idx < cd.initialiserList.size() ; ++idx) {
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
    const atl::shared_ptr<X64::Operand> argReg = fc.funArgs[argNum]->accept(*this);
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
    const atl::shared_ptr<X64::Operand> argAddr = fd.funParams[idx]->accept(*this);
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
  // x64.cmp(condResReg, 0);
  x64.jeq(falseBlockName);
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
  const atl::vector<atl::shared_ptr<Decl>> classDecls = objClassTypeDef->classDecls;
  unsigned int objByteOffset = 0u;
  for (unsigned int i = 0u; i < classDecls.size(); ++i) {
    const atl::shared_ptr<Decl> currDecl = classDecls[i];
    if (currDecl->astClass() != "VarDecl" && currDecl->astClass() != "VarDef") {
      continue;
    }
    const atl::shared_ptr<VarDecl> currMember = atl::static_pointer_cast<VarDecl>(currDecl);
    if (currMember == ma.fieldVariable->varDecl.lock()) {
      break;
    }
    objByteOffset += currMember->getBytes();
  }

  return atl::shared_ptr<X64::AddrOffset>(new X64::AddrOffset(objAddr, objByteOffset));
}
atl::shared_ptr<X64::Operand> GenerateX64::visit(MemberCall &mc) {
  atl::stack<atl::shared_ptr<X64::Register>> paramRegs = x64.paramRegs();

  x64.callerPrologue();

  /* Handle `this` parameter. */
  const atl::shared_ptr<X64::Operand> this_ptr =  mc.object->accept(*this);
  x64.mov(paramRegs.pop_back(), this_ptr);

  const FunCall &fc = *mc.funCall;
  for (unsigned int argNum = 0u; argNum < fc.funArgs.size(); ++argNum) {
    const atl::shared_ptr<X64::Operand> argReg = fc.funArgs[argNum]->accept(*this);
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
  // TODO
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
  x64.string_literal(strName, strVal);
  return atl::shared_ptr<X64::StringLiteral>(
      new X64::StringLiteral(strName, strVal));
}
atl::shared_ptr<X64::Operand> GenerateX64::visit(SubscriptOp &so) {
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
  va.derefExpr->accept(*this);
  return atl::shared_ptr<X64::None>();
}
atl::shared_ptr<X64::Operand> GenerateX64::visit(VarDecl &vd) {
  int bytesRequired = vd.getBytes();
  while (bytesRequired % 16 != 0)
    ++bytesRequired;
  vd.bpOffset = currBpOffset;
  currBpOffset -= bytesRequired;

  const atl::string comment =
      "Allocated " + atl::to_string(vd.getBytes()) +
      "B (16B Aligned) for VarDecl: " + vd.getIdentifier()->toString() +
      " @ [rbp" + atl::to_string(currBpOffset) + "]";

  x64.sub(x64.rsp, bytesRequired, comment);
  return atl::shared_ptr<X64::AddrOffset>(new X64::AddrOffset(x64.rbp, vd.bpOffset));
  // return atl::shared_ptr<X64::None>();
}
atl::shared_ptr<X64::Operand> GenerateX64::visit(VarDef &vd) {
  const atl::string vdIdent = vd.getIdentifier()->toString();
  int bytesRequired = vd.getBytes();
  while (bytesRequired % 16 != 0)
    ++bytesRequired;
  vd.bpOffset = currBpOffset;
  currBpOffset -= bytesRequired;

  const atl::string comment = "Allocated " + atl::to_string(vd.getBytes()) +
                              "B (16B Aligned) for VarDef: " + vdIdent +
                              " @ [rbp" + atl::to_string(currBpOffset) + "]";

  x64.sub(x64.rsp, bytesRequired, comment);

  const atl::shared_ptr<X64::Operand> valueOperand = vd.varValue->accept(*this);

  if (valueOperand->opType() != "None") {
    x64.mov(x64.rax, valueOperand,
            "Move " + vdIdent + "'s value into temp register.");
    x64.mov(addrOffset(x64.rbp, vd.bpOffset), x64.rax,
            "Move " + vdIdent + "'s temp register into its stack allocated space.");
  }

  return atl::shared_ptr<X64::AddrOffset>(new X64::AddrOffset(x64.rbp, vd.bpOffset));
}
atl::shared_ptr<X64::Operand> GenerateX64::visit(VarExpr &ve) {
  /* Find this Variable's Location in the Stack, and Load It. */
  const atl::shared_ptr<VarDecl> varDecl = ve.varDecl.lock();
  if (varDecl->isGlobal) {
    return atl::shared_ptr<X64::GlobalVariable>(new X64::GlobalVariable(
        ve.varDecl.lock()->getIdentifier()->toString(), ve.varDecl.lock()->getBytes()));
  }

  return addrOffset(x64.rbp, varDecl->bpOffset);
}
atl::shared_ptr<X64::Operand> GenerateX64::visit(While &w) {
  w.condition->accept(*this);
  w.body->accept(*this);
  return atl::shared_ptr<X64::None>();
}

/* ---- Helpers ---- */

atl::shared_ptr<X64::IntValue> GenerateX64::genIntValue(int value) {
  return atl::shared_ptr<X64::IntValue>(new X64::IntValue(atl::to_string(value)));
}

atl::shared_ptr<X64::AddrOffset>
GenerateX64::addrOffset(const atl::shared_ptr<X64::Operand> addrOperand,
                        const int offset) {
  return atl::shared_ptr<X64::AddrOffset>(
      new X64::AddrOffset(addrOperand, offset));
}
