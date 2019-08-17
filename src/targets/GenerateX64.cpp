#include "targets/GenerateX64.h"

using namespace ACC;

GenerateX64::GenerateX64(atl::shared_ptr<Program> progAST,
                         const atl::string &outputFile)
    : x64(outputFile), progAST(progAST) {}

void GenerateX64::error(atl::string error) {
  errorCount++;
  errors.push_back(error);
}

void GenerateX64::printErrors() const {
  printf("FATAL x86 Generation Errors:\n");
  for (unsigned int idx = 0; idx < errors.size(); ++idx)
    printf("\t%s\n", errors[idx].c_str());
}

void GenerateX64::run() { visit(*progAST); }

void GenerateX64::alloc(const VarDecl &vd) {
  x64.write(vd.getIdentifier()->toString() + ": db " +
            atl::to_string(vd.getBytes()));
}

/* ---- Visit AST ---- */

atl::shared_ptr<X64::Operand> GenerateX64::visit(AddressOf &ao) {
  return atl::make_shared<X64::None>();
}
atl::shared_ptr<X64::Operand> GenerateX64::visit(Allocation &a) {
  return atl::make_shared<X64::None>();
}
atl::shared_ptr<X64::Operand> GenerateX64::visit(ArrayType &at) {
  return atl::make_shared<X64::None>();
}
atl::shared_ptr<X64::Operand> GenerateX64::visit(Assign &as) {
  atl::shared_ptr<X64::Operand> lhsRegRes = as.lhs->accept(*this);
  atl::shared_ptr<X64::Operand> rhsResReg = as.rhs->accept(*this);
  x64.mov(lhsRegRes, rhsResReg);
  return atl::make_shared<X64::None>();
}
atl::shared_ptr<X64::Operand> GenerateX64::visit(BaseType &bt) {
  return atl::make_shared<X64::None>();
}
atl::shared_ptr<X64::Operand> GenerateX64::visit(BinOp &bo) {
  /* Evaluate LHS and Store to Stack */
  atl::shared_ptr<X64::Operand> lhsOperand = bo.lhs->accept(*this);
  if (lhsOperand->opType() != "GlobalVariable")
    x64.push(lhsOperand, "Store LHS to Stack");
  else
    x64.comment("LHS is a GlobalVariable: " + lhsOperand->toString());

  /* Evaluate RHS and Store in EAX */
  atl::shared_ptr<X64::Operand> rhsOperand = bo.rhs->accept(*this);

  if (rhsOperand->opType() != "GlobalVariable")
    x64.pop(X64::rcx, "Restore LHS from Stack");
  else
    x64.mov(X64::rcx, rhsOperand,
            "Restore LHS from GlobalVariable: " + rhsOperand->toString());

  /* Perform BinOp */
  switch (bo.operation) {
  case Op::ADD: {
    x64.add(X64::rax, X64::rcx);
    break;
  }
  case Op::MUL: {
    x64.imul(X64::rax, X64::rcx);
    break;
  }
  default:
    x64.comment("Not Implemented this BinOp Yet");
    break;
  }
  return X64::rax;
}
atl::shared_ptr<X64::Operand> GenerateX64::visit(Block &b) {
  currScope = b.getptr();
  for (unsigned int idx = 0; idx < b.stmts.size(); ++idx)
    b.stmts[idx]->accept(*this);

  currScope = b.outerScope;
  return atl::make_shared<X64::None>();
}
atl::shared_ptr<X64::Operand> GenerateX64::visit(BoolLiteral &bl) {
  return atl::make_shared<X64::None>();
}
atl::shared_ptr<X64::Operand> GenerateX64::visit(CharLiteral &cl) {
  return atl::make_shared<X64::None>();
}
atl::shared_ptr<X64::Operand> GenerateX64::visit(ClassType &ct) {
  return atl::make_shared<X64::None>();
}
atl::shared_ptr<X64::Operand> GenerateX64::visit(ClassTypeDecl &ctd) {
  return atl::make_shared<X64::None>();
}
atl::shared_ptr<X64::Operand> GenerateX64::visit(ClassTypeDef &ctd) {
  for (unsigned int idx = 0u; idx < ctd.classDecls.size(); ++idx) {
    ctd.classDecls[idx]->accept(*this);
  }

  return atl::make_shared<X64::None>();
}
atl::shared_ptr<X64::Operand> GenerateX64::visit(ConstructorCall &cc) {
  // TODO: Allocate for the type we are about to construct.
  for (int idx = cc.constructorArgs.size() - 1; idx >= 0; --idx) {
    atl::shared_ptr<X64::Operand> argReg =
        cc.constructorArgs[idx]->accept(*this);
    x64.push(argReg);
  }
  x64.call(cc.constructorIdentifier->mangle());
  return X64::rax;
}
atl::shared_ptr<X64::Operand> GenerateX64::visit(ConstructorDecl &cd) {
  return atl::make_shared<X64::None>();
}
atl::shared_ptr<X64::Operand> GenerateX64::visit(ConstructorDef &cd) {
  x64.block("CtorDecl_" + cd.getSignature().mangle());
  return atl::make_shared<X64::None>();
}
atl::shared_ptr<X64::Operand> GenerateX64::visit(Deletion &d) {
  return atl::make_shared<X64::None>();
}
atl::shared_ptr<X64::Operand> GenerateX64::visit(DestructorDecl &dd) {
  return atl::make_shared<X64::None>();
}
atl::shared_ptr<X64::Operand> GenerateX64::visit(DestructorDef &dd) {
  return atl::make_shared<X64::None>();
}
atl::shared_ptr<X64::Operand> GenerateX64::visit(DoWhile &dw) {
  dw.body->accept(*this);
  dw.condition->accept(*this);
  return atl::make_shared<X64::None>();
}
atl::shared_ptr<X64::Operand> GenerateX64::visit(EnumClassTypeDecl &ectd) {
  return atl::make_shared<X64::None>();
}
atl::shared_ptr<X64::Operand> GenerateX64::visit(For &f) {
  return atl::make_shared<X64::None>();
}
atl::shared_ptr<X64::Operand> GenerateX64::visit(FunCall &fc) {
  const atl::vector<atl::shared_ptr<X64::Register>> argRegisters = {
      X64::rdi, X64::rsi, X64::rdx,
      X64::rcx // R8, R9
  };
  for (unsigned int argNum = 0u; argNum < fc.funArgs.size(); ++argNum) {
    const atl::shared_ptr<X64::Operand> argReg =
        fc.funArgs[argNum]->accept(*this);
    if (argNum < 4)
      x64.mov(argRegisters[argNum], argReg);
    else
      x64.push(argReg);
  }
  x64.call(fc.funDecl->getSignature().mangle());
  return X64::rax;
}
atl::shared_ptr<X64::Operand> GenerateX64::visit(FunDecl &fd) {
  // currScope = fd.funBlock;

  // x64.block(fd.getIdentifier() + "FunDecl");

  // /* ---- Callee Prologue ---- */
  // x64.push(X64::rbp);
  // x64.mov(X64::rbp, X64::rsp);
  // x64.push(X64::rbx);
  // x64.push(X64::rdi);
  // x64.push(X64::rsi);

  // /* ---- Execute Function ---- */
  // x64.comment(" ---- Function Body ----");
  // fd.funBlock->accept(*this);
  // x64.comment(" -----------------------");

  // /* -------------------------- */

  // /* ---- Callee Epilogue ---- */
  // x64.pop(X64::rsi);
  // x64.pop(X64::rdi);
  // x64.pop(X64::rbx);
  // x64.mov(X64::rsp, X64::rbp);
  // x64.pop(X64::rbp);
  // x64.ret();
  // x64.write("");

  // currFpOffset = 0;
  // currScope = fd.funBlock->outerBlock;
  return atl::make_shared<X64::None>(X64::None());
}
atl::shared_ptr<X64::Operand> GenerateX64::visit(FunDef &fd) {
  currScope = fd.funBlock;

  x64.block("FunDecl_" + fd.getSignature().mangle());

  /* ---- Callee Prologue ---- */
  x64.push(X64::rbp);
  x64.mov(X64::rbp, X64::rsp);
  x64.push(X64::rbx);
  x64.push(X64::rdi);
  x64.push(X64::rsi);

  /* ---- Execute Function ---- */
  x64.comment(" ---- Function Args ----");
  for (unsigned int idx = 0; idx < fd.funParams.size(); ++idx)
    fd.funParams[idx]->accept(*this);
  x64.comment(" ---- Function Body ----");

  if (fd.getIdentifier()->value == "main")
    fd.funBlock->accept(*this);
  x64.comment(" -----------------------");

  /* -------------------------- */

  /* ---- Callee Epilogue ---- */
  x64.pop(X64::rsi);
  x64.pop(X64::rdi);
  x64.pop(X64::rbx);
  x64.mov(X64::rsp, X64::rbp);
  x64.pop(X64::rbp);
  x64.ret();
  x64.write("");

  currFpOffset = 0;
  currScope = fd.funBlock->outerScope;
  return atl::make_shared<X64::None>(X64::None());
}
atl::shared_ptr<X64::Operand> GenerateX64::visit(Identifier &i) {
  return atl::make_shared<X64::None>();
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
  return atl::make_shared<X64::None>();
}
atl::shared_ptr<X64::Operand> GenerateX64::visit(IntLiteral &il) {
  x64.mov(X64::rax,
          atl::make_shared<X64::IntValue>(X64::IntValue(il.getLiteral())));
  return X64::rax;
}
atl::shared_ptr<X64::Operand> GenerateX64::visit(MemberAccess &ma) {
  ma.object->accept(*this);
  return atl::make_shared<X64::None>();
}
atl::shared_ptr<X64::Operand> GenerateX64::visit(MemberCall &mc) {
  mc.object->accept(*this);
  return atl::make_shared<X64::None>();
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
  return atl::make_shared<X64::None>();
}
atl::shared_ptr<X64::Operand> GenerateX64::visit(PrefixOp &po) {
  // TODO
  return atl::make_shared<X64::None>();
}
atl::shared_ptr<X64::Operand> GenerateX64::visit(Program &p) {
  currScope = p.getptr();

  x64.write("SECTION .data");
  for (unsigned int idx = 0; idx < p.globalVars.size(); ++idx) {
    const atl::shared_ptr<VarDecl> &globalVar = p.globalVars[idx];
    alloc(*globalVar);
  }

  x64.write("SECTION .text");

  x64.write("global _main");
  for (unsigned int idx = 0u; idx < X64::externFunDecls.size(); ++idx) {
    const atl::string &externFunDecl = X64::externFunDecls[idx];
    x64.write("extern _" + externFunDecl);
  }

  x64.block("_main");
  x64.call("main_int__char_ptr_ptr_");
  x64.ret();

  x64.block("FunDecl_printf_char_ptr__char_ptr_");
  // x64.call("_printf");
  x64.write("call _printf");
  x64.ret();

  for (unsigned int idx = 0u; idx < p.decls.size(); ++idx) {
    x64.write("\n\n");
    p.decls[idx]->accept(*this);
    x64.write("\n\n");
  }

  return atl::make_shared<X64::None>();
}
atl::shared_ptr<X64::Operand> GenerateX64::visit(ReferenceType &rt) {
  return atl::make_shared<X64::None>();
}
atl::shared_ptr<X64::Operand> GenerateX64::visit(Return &r) {
  if (r.returnExpr) {
    atl::shared_ptr<X64::Operand> rVal = r.returnExpr->accept(*this);
    x64.mov(X64::rax, rVal);
  }
  return atl::make_shared<X64::None>();
}
atl::shared_ptr<X64::Operand> GenerateX64::visit(SizeOf &so) {
  return atl::make_shared<X64::None>();
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
  return atl::make_shared<X64::None>();
}
atl::shared_ptr<X64::Operand> GenerateX64::visit(TemplateDef &td) {
  return atl::make_shared<X64::None>();
}
atl::shared_ptr<X64::Operand> GenerateX64::visit(TertiaryExpr &t) {
  return atl::make_shared<X64::None>();
}
atl::shared_ptr<X64::Operand> GenerateX64::visit(Throw &t) {
  return atl::make_shared<X64::None>();
}
atl::shared_ptr<X64::Operand> GenerateX64::visit(TypeDefDecl &td) {
  return atl::make_shared<X64::None>();
}
atl::shared_ptr<X64::Operand> GenerateX64::visit(ValueAt &va) {
  va.derefExpr->accept(*this);
  return atl::make_shared<X64::None>();
}
atl::shared_ptr<X64::Operand> GenerateX64::visit(VarDecl &vd) {
  int bytesRequired = vd.getBytes();
  while (bytesRequired % 16 != 0)
    ++bytesRequired;
  currFpOffset -= bytesRequired;
  vd.fpOffset = currFpOffset;

  const atl::string comment =
      "Allocated " + atl::to_string(vd.getBytes()) +
      "B (16B Aligned) for VarDecl: " + vd.getIdentifier()->toString() +
      " @ [rbp" + atl::to_string(currFpOffset) + "]";

  x64.sub(X64::rsp, bytesRequired, comment);
  return atl::make_shared<X64::None>();
}
atl::shared_ptr<X64::Operand> GenerateX64::visit(VarDef &vd) {
  const atl::string vdIdent = vd.getIdentifier()->toString();
  int bytesRequired = vd.getBytes();
  while (bytesRequired % 16 != 0)
    ++bytesRequired;
  currFpOffset -= bytesRequired;
  vd.fpOffset = currFpOffset;

  const atl::string comment = "Allocated " + atl::to_string(vd.getBytes()) +
                              "B (16B Aligned) for VarDef: " + vdIdent +
                              " @ [rbp" + atl::to_string(currFpOffset) + "]";

  x64.sub(X64::rsp, bytesRequired, comment);

  // Operand should be an AddrOffset.
  if (vd.varValue->astClass() == "ConstructorCall") {
    currObject = atl::shared_ptr<VarExpr>(new VarExpr(vd.getIdentifier()));
    currObject->varDecl = vd.getptr();
  }

  const atl::shared_ptr<X64::Operand> valueOperand = vd.varValue->accept(*this);

  x64.mov(X64::rax, valueOperand,
          "Move " + vdIdent + "'s value into temp register.");
  x64.mov(addrOffset(X64::rbp, vd.fpOffset), X64::rax,
          "Move " + vdIdent +
              "'s temp register into its stack allocated space.");

  return atl::make_shared<X64::None>();
}
atl::shared_ptr<X64::Operand> GenerateX64::visit(VarExpr &ve) {
  /* Find this Variable's Location in the Stack, and Load It. */
  const int fpOffset = ve.varDecl->fpOffset;
  if (fpOffset == 0)
    return atl::make_shared<X64::GlobalVariable>(X64::GlobalVariable(
        ve.varDecl->getIdentifier()->toString(), ve.varDecl->getBytes()));

  return addrOffset(X64::rbp, fpOffset);
}
atl::shared_ptr<X64::Operand> GenerateX64::visit(While &w) {
  w.condition->accept(*this);
  w.body->accept(*this);
  return atl::make_shared<X64::None>();
}

/* ---- Helpers ---- */

atl::shared_ptr<X64::IntValue> GenerateX64::genIntValue(int value) {
  return atl::make_shared<X64::IntValue>(X64::IntValue(atl::to_string(value)));
}

atl::shared_ptr<X64::AddrOffset>
GenerateX64::addrOffset(const atl::shared_ptr<X64::Operand> addrOperand,
                        const int offset) {
  return atl::shared_ptr<X64::AddrOffset>(
      new X64::AddrOffset(addrOperand, offset));
}
