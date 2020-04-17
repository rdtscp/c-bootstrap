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
  printf("FATAL x64 Generation Errors:\n");
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
  x64.mov(lhsRegRes, rhsResReg);
  return atl::shared_ptr<X64::None>();
}
atl::shared_ptr<X64::Operand> GenerateX64::visit(BaseType &bt) {
  return atl::shared_ptr<X64::None>();
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
    x64.pop(X64::ecx, "Restore LHS from Stack");
  else
    x64.mov(X64::ecx, rhsOperand,
            "Restore LHS from GlobalVariable: " + rhsOperand->toString());

  /* Perform BinOp */
  switch (bo.operation) {
  case Op::ADD: {
    x64.add(X64::eax, X64::ecx);
    break;
  }
  case Op::MUL: {
    x64.imul(X64::eax, X64::ecx);
    break;
  }
  default:
    x64.comment("Not Implemented this BinOp Yet");
    break;
  }
  return X64::eax;
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
  return atl::shared_ptr<X64::None>();
}
atl::shared_ptr<X64::Operand> GenerateX64::visit(ConstructorCall &cc) {
  for (int idx = cc.constructorArgs.size() - 1; idx >= 0; --idx) {
    atl::shared_ptr<X64::Operand> argReg =
        cc.constructorArgs[idx]->accept(*this);
    // x64.push(argReg);
  }
  x64.call(cc.constructorIdentifier->toString());
  return X64::eax;
}
atl::shared_ptr<X64::Operand> GenerateX64::visit(ConstructorDecl &cd) {
  return atl::shared_ptr<X64::None>();
}
atl::shared_ptr<X64::Operand> GenerateX64::visit(ConstructorDef &cd) {
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
  for (int idx = fc.funArgs.size() - 1; idx >= 0; --idx) {
    atl::shared_ptr<X64::Operand> argReg = fc.funArgs[idx]->accept(*this);
    // x64.push(argReg);
  }
  x64.call(fc.funIdentifier->toString());
  return X64::eax;
}
atl::shared_ptr<X64::Operand> GenerateX64::visit(FunDecl &fd) {
  // currScope = fd.funBlock;

  // x64.block(fd.getIdentifier() + "FunDecl");

  // /* ---- Callee Prologue ---- */
  // x64.push(X64::ebp);
  // x64.mov(X64::ebp, X64::esp);
  // x64.push(X64::ebx);
  // x64.push(X64::edi);
  // x64.push(X64::esi);

  // /* ---- Execute Function ---- */
  // x64.comment(" ---- Function Body ----");
  // fd.funBlock->accept(*this);
  // x64.comment(" -----------------------");

  // /* -------------------------- */

  // /* ---- Callee Epilogue ---- */
  // x64.pop(X64::esi);
  // x64.pop(X64::edi);
  // x64.pop(X64::ebx);
  // x64.mov(X64::esp, X64::ebp);
  // x64.pop(X64::ebp);
  // x64.ret();
  // x64.write("");

  // currFpOffset = 0;
  // currScope = fd.funBlock->outerBlock;
  return atl::shared_ptr<X64::None>();
}
atl::shared_ptr<X64::Operand> GenerateX64::visit(FunDef &fd) {
  currScope = fd.funBlock;

  x64.block(fd.getIdentifier()->toString() + "FunDecl");

  /* ---- Callee Prologue ---- */
  x64.push(X64::ebp);
  x64.mov(X64::ebp, X64::esp);
  x64.push(X64::ebx);
  x64.push(X64::edi);
  x64.push(X64::esi);

  /* ---- Execute Function ---- */
  x64.comment(" ---- Function Body ----");
  fd.funBlock->accept(*this);
  x64.comment(" -----------------------");

  /* -------------------------- */

  /* ---- Callee Epilogue ---- */
  x64.pop(X64::esi);
  x64.pop(X64::edi);
  x64.pop(X64::ebx);
  x64.mov(X64::esp, X64::ebp);
  x64.pop(X64::ebp);
  x64.ret();
  x64.write("");

  currFpOffset = 0;
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
  x64.mov(X64::eax,
          atl::shared_ptr<X64::IntValue>(new X64::IntValue(il.getLiteral())));
  return X64::eax;
}
atl::shared_ptr<X64::Operand> GenerateX64::visit(MemberAccess &ma) {
  ma.object->accept(*this);
  return atl::shared_ptr<X64::None>();
}
atl::shared_ptr<X64::Operand> GenerateX64::visit(MemberCall &mc) {
  mc.object->accept(*this);
  return atl::shared_ptr<X64::None>();
}
atl::shared_ptr<X64::Operand> GenerateX64::visit(Namespace &n) {
  // const unsigned int numDecls = n.namespaceDecls.size();
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

  x64.write("SECTION .data");
  for (unsigned int idx = 0; idx < p.globalVars.size(); ++idx) {
    const atl::shared_ptr<VarDecl> &globalVar = p.globalVars[idx];
    alloc(*globalVar);
  }

  x64.write("SECTION .text");

  x64.write("global _main");

  x64.block("_main");
  x64.call("main");
  x64.ret();

  for (unsigned int idx = 0; idx < p.funDecls.size(); ++idx)
    p.funDecls[idx]->accept(*this);

  return atl::shared_ptr<X64::None>();
}
atl::shared_ptr<X64::Operand> GenerateX64::visit(ReferenceType &rt) {
  return atl::shared_ptr<X64::None>();
}
atl::shared_ptr<X64::Operand> GenerateX64::visit(Return &r) {
  if (r.returnExpr) {
    atl::shared_ptr<X64::Operand> rVal = r.returnExpr->accept(*this);
    x64.mov(X64::eax, rVal);
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
  return atl::shared_ptr<X64::None>();
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
  currFpOffset -= bytesRequired;
  vd.fpOffset = currFpOffset;

  const atl::string comment = "Allocated " + atl::to_string(bytesRequired) +
                              "B for VarDecl: " + vd.getIdentifier() +
                              " @ [ebp" + atl::to_string(currFpOffset) + "]";

  x64.sub(X64::esp, bytesRequired, comment);
  return atl::shared_ptr<X64::None>();
}
atl::shared_ptr<X64::Operand> GenerateX64::visit(VarDef &vd) {
  int bytesRequired = vd.getBytes();
  currFpOffset -= bytesRequired;
  vd.fpOffset = currFpOffset;

  const atl::string comment = "Allocated " + atl::to_string(bytesRequired) +
                              "B for VarDef: " + vd.getIdentifier() +
                              " @ [ebp" + atl::to_string(currFpOffset) + "]";

  x64.sub(X64::esp, bytesRequired, comment);
  return atl::shared_ptr<X64::None>();
}
atl::shared_ptr<X64::Operand> GenerateX64::visit(VarExpr &ve) {
  /* Find this Variable's Location in the Stack, and Load It. */
  const int fpOffset = ve.varDecl.lock()->fpOffset;
  if (fpOffset == 0)
    return atl::shared_ptr<X64::GlobalVariable>(new X64::GlobalVariable(
        ve.varDecl.lock()->getIdentifier()->toString(), ve.varDecl.lock()->getBytes()));

  if (fpOffset > 0)
    return atl::shared_ptr<X64::Register>(new 
        X64::Register(0, "[ebp+" + atl::to_string(fpOffset) + "]"));
  else
    return atl::shared_ptr<X64::Register>(new 
        X64::Register(0, "[ebp" + atl::to_string(fpOffset) + "]"));
}
atl::shared_ptr<X64::Operand> GenerateX64::visit(While &w) {
  w.condition->accept(*this);
  w.body->accept(*this);
  return atl::shared_ptr<X64::None>();
}

/* ---- Helpers ---- */

atl::shared_ptr<X64::Operand> GenerateX64::genIntValue(int value) {
  return atl::shared_ptr<X64::IntValue>(new X64::IntValue(atl::to_string(value)));
}