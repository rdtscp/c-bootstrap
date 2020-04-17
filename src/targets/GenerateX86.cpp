#include "targets/GenerateX86.h"

using namespace ACC;

GenerateX86::GenerateX86(atl::shared_ptr<Program> progAST,
                         const atl::string &outputFile)
    : x86(outputFile), progAST(progAST) {}

void GenerateX86::error(atl::string error) {
  errorCount++;
  errors.push_back(error);
}

void GenerateX86::printErrors() const {
  printf("FATAL x86 Generation Errors:\n");
  for (unsigned int idx = 0; idx < errors.size(); ++idx)
    printf("\t%s\n", errors[idx].c_str());
}

void GenerateX86::run() { visit(*progAST); }

void GenerateX86::alloc(const VarDecl &vd) {
  x86.write(vd.getIdentifier()->toString() + ": db " +
            atl::to_string(vd.getBytes()));
}

/* ---- Visit AST ---- */

atl::shared_ptr<X86::Operand> GenerateX86::visit(AddressOf &ao) {
  return atl::shared_ptr<X86::None>();
}
atl::shared_ptr<X86::Operand> GenerateX86::visit(Allocation &a) {
  return atl::shared_ptr<X86::None>();
}
atl::shared_ptr<X86::Operand> GenerateX86::visit(ArrayType &at) {
  return atl::shared_ptr<X86::None>();
}
atl::shared_ptr<X86::Operand> GenerateX86::visit(Assign &as) {
  atl::shared_ptr<X86::Operand> lhsRegRes = as.lhs->accept(*this);
  atl::shared_ptr<X86::Operand> rhsResReg = as.rhs->accept(*this);
  x86.mov(lhsRegRes, rhsResReg);
  return atl::shared_ptr<X86::None>();
}
atl::shared_ptr<X86::Operand> GenerateX86::visit(BaseType &bt) {
  return atl::shared_ptr<X86::None>();
}
atl::shared_ptr<X86::Operand> GenerateX86::visit(BinOp &bo) {
  /* Evaluate LHS and Store to Stack */
  atl::shared_ptr<X86::Operand> lhsOperand = bo.lhs->accept(*this);
  if (lhsOperand->opType() != "GlobalVariable")
    x86.push(lhsOperand, "Store LHS to Stack");
  else
    x86.comment("LHS is a GlobalVariable: " + lhsOperand->toString());

  /* Evaluate RHS and Store in EAX */
  atl::shared_ptr<X86::Operand> rhsOperand = bo.rhs->accept(*this);

  if (rhsOperand->opType() != "GlobalVariable")
    x86.pop(X86::ecx, "Restore LHS from Stack");
  else
    x86.mov(X86::ecx, rhsOperand,
            "Restore LHS from GlobalVariable: " + rhsOperand->toString());

  /* Perform BinOp */
  switch (bo.operation) {
  case Op::ADD: {
    x86.add(X86::eax, X86::ecx);
    break;
  }
  case Op::MUL: {
    x86.imul(X86::eax, X86::ecx);
    break;
  }
  default:
    x86.comment("Not Implemented this BinOp Yet");
    break;
  }
  return X86::eax;
}
atl::shared_ptr<X86::Operand> GenerateX86::visit(Block &b) {
  currScope = b.getptr();
  for (unsigned int idx = 0; idx < b.stmts.size(); ++idx)
    b.stmts[idx]->accept(*this);

  currScope = b.outerScope.lock();
  return atl::shared_ptr<X86::None>();
}
atl::shared_ptr<X86::Operand> GenerateX86::visit(BoolLiteral &bl) {
  return atl::shared_ptr<X86::None>();
}
atl::shared_ptr<X86::Operand> GenerateX86::visit(CharLiteral &cl) {
  return atl::shared_ptr<X86::None>();
}
atl::shared_ptr<X86::Operand> GenerateX86::visit(ClassType &ct) {
  return atl::shared_ptr<X86::None>();
}
atl::shared_ptr<X86::Operand> GenerateX86::visit(ClassTypeDecl &ctd) {
  return atl::shared_ptr<X86::None>();
}
atl::shared_ptr<X86::Operand> GenerateX86::visit(ClassTypeDef &ctd) {
  return atl::shared_ptr<X86::None>();
}
atl::shared_ptr<X86::Operand> GenerateX86::visit(ConstructorCall &cc) {
  for (int idx = cc.constructorArgs.size() - 1; idx >= 0; --idx) {
    atl::shared_ptr<X86::Operand> argReg =
        cc.constructorArgs[idx]->accept(*this);
    // x86.push(argReg);
  }
  x86.call(cc.constructorIdentifier->toString());
  return X86::eax;
}
atl::shared_ptr<X86::Operand> GenerateX86::visit(ConstructorDecl &cd) {
  return atl::shared_ptr<X86::None>();
}
atl::shared_ptr<X86::Operand> GenerateX86::visit(ConstructorDef &cd) {
  return atl::shared_ptr<X86::None>();
}
atl::shared_ptr<X86::Operand> GenerateX86::visit(Deletion &d) {
  return atl::shared_ptr<X86::None>();
}
atl::shared_ptr<X86::Operand> GenerateX86::visit(DestructorDecl &dd) {
  return atl::shared_ptr<X86::None>();
}
atl::shared_ptr<X86::Operand> GenerateX86::visit(DestructorDef &dd) {
  return atl::shared_ptr<X86::None>();
}
atl::shared_ptr<X86::Operand> GenerateX86::visit(DoWhile &dw) {
  dw.body->accept(*this);
  dw.condition->accept(*this);
  return atl::shared_ptr<X86::None>();
}
atl::shared_ptr<X86::Operand> GenerateX86::visit(EnumClassTypeDecl &ectd) {
  return atl::shared_ptr<X86::None>();
}
atl::shared_ptr<X86::Operand> GenerateX86::visit(For &f) {
  return atl::shared_ptr<X86::None>();
}
atl::shared_ptr<X86::Operand> GenerateX86::visit(FunCall &fc) {
  for (int idx = fc.funArgs.size() - 1; idx >= 0; --idx) {
    atl::shared_ptr<X86::Operand> argReg = fc.funArgs[idx]->accept(*this);
    // x86.push(argReg);
  }
  x86.call(fc.funIdentifier->toString());
  return X86::eax;
}
atl::shared_ptr<X86::Operand> GenerateX86::visit(FunDecl &fd) {
  // currScope = fd.funBlock;

  // x86.block(fd.getIdentifier() + "FunDecl");

  // /* ---- Callee Prologue ---- */
  // x86.push(X86::ebp);
  // x86.mov(X86::ebp, X86::esp);
  // x86.push(X86::ebx);
  // x86.push(X86::edi);
  // x86.push(X86::esi);

  // /* ---- Execute Function ---- */
  // x86.comment(" ---- Function Body ----");
  // fd.funBlock->accept(*this);
  // x86.comment(" -----------------------");

  // /* -------------------------- */

  // /* ---- Callee Epilogue ---- */
  // x86.pop(X86::esi);
  // x86.pop(X86::edi);
  // x86.pop(X86::ebx);
  // x86.mov(X86::esp, X86::ebp);
  // x86.pop(X86::ebp);
  // x86.ret();
  // x86.write("");

  // currFpOffset = 0;
  // currScope = fd.funBlock->outerBlock;
  return atl::shared_ptr<X86::None>();
}
atl::shared_ptr<X86::Operand> GenerateX86::visit(FunDef &fd) {
  currScope = fd.funBlock;

  x86.block(fd.getIdentifier()->toString() + "FunDecl");

  /* ---- Callee Prologue ---- */
  x86.push(X86::ebp);
  x86.mov(X86::ebp, X86::esp);
  x86.push(X86::ebx);
  x86.push(X86::edi);
  x86.push(X86::esi);

  /* ---- Execute Function ---- */
  x86.comment(" ---- Function Body ----");
  fd.funBlock->accept(*this);
  x86.comment(" -----------------------");

  /* -------------------------- */

  /* ---- Callee Epilogue ---- */
  x86.pop(X86::esi);
  x86.pop(X86::edi);
  x86.pop(X86::ebx);
  x86.mov(X86::esp, X86::ebp);
  x86.pop(X86::ebp);
  x86.ret();
  x86.write("");

  currFpOffset = 0;
  currScope = fd.funBlock->outerScope.lock();
  return atl::shared_ptr<X86::None>();
}
atl::shared_ptr<X86::Operand> GenerateX86::visit(Identifier &i) {
  return atl::shared_ptr<X86::None>();
}
atl::shared_ptr<X86::Operand> GenerateX86::visit(If &i) {
  /* Calculate Names for Blocks */
  const atl::string trueBlockName =
      "ifTrueBlock" + atl::to_string(blockCount++);
  const atl::string falseBlockName =
      "ifFalseBlock" + atl::to_string(blockCount++);
  const atl::string endBlockName = "ifEndBlock" + atl::to_string(blockCount++);

  /* Calculate the result of the if condition. */
  atl::shared_ptr<X86::Operand> condResReg = i.ifCondition->accept(*this);

  /* Branch to False block if False, else branch to True block. */
  // x86.cmp(condResReg, 0);
  x86.jeq(falseBlockName);
  x86.jmp(trueBlockName);

  /* Handle when the Case is True. */
  x86.block(trueBlockName);
  i.ifBody->accept(*this);
  x86.jmp(endBlockName);

  /* Handle when the Case is False. */
  x86.block(falseBlockName);
  if (i.elseBody)
    i.elseBody->accept(*this);
  x86.jmp(endBlockName);

  /* Handle after the If statement. */
  x86.block(endBlockName);
  return atl::shared_ptr<X86::None>();
}
atl::shared_ptr<X86::Operand> GenerateX86::visit(IntLiteral &il) {
  x86.mov(X86::eax,
          atl::shared_ptr<X86::IntValue>(new X86::IntValue(il.getLiteral())));
  return X86::eax;
}
atl::shared_ptr<X86::Operand> GenerateX86::visit(MemberAccess &ma) {
  ma.object->accept(*this);
  return atl::shared_ptr<X86::None>();
}
atl::shared_ptr<X86::Operand> GenerateX86::visit(MemberCall &mc) {
  mc.object->accept(*this);
  return atl::shared_ptr<X86::None>();
}
atl::shared_ptr<X86::Operand> GenerateX86::visit(Namespace &n) {
  // const unsigned int numDecls = n.namespaceDecls.size();
  return nullptr;
}

atl::shared_ptr<X86::Operand> GenerateX86::visit(Nullptr &n) {
  return genIntValue(0);
}
atl::shared_ptr<X86::Operand> GenerateX86::visit(ParenthExpr &pe) {
  return pe.innerExpr->accept(*this);
}
atl::shared_ptr<X86::Operand> GenerateX86::visit(PointerType &pt) {
  return atl::shared_ptr<X86::None>();
}
atl::shared_ptr<X86::Operand> GenerateX86::visit(PrefixOp &po) {
  // TODO
  return atl::shared_ptr<X86::None>();
}
atl::shared_ptr<X86::Operand> GenerateX86::visit(Program &p) {
  currScope = p.getptr();

  x86.write("SECTION .data");
  for (unsigned int idx = 0; idx < p.globalVars.size(); ++idx) {
    const atl::shared_ptr<VarDecl> &globalVar = p.globalVars[idx];
    alloc(*globalVar);
  }

  x86.write("SECTION .text");

  x86.write("global _main");

  x86.block("_main");
  x86.call("main");
  x86.ret();

  for (unsigned int idx = 0; idx < p.funDecls.size(); ++idx)
    p.funDecls[idx]->accept(*this);

  return atl::shared_ptr<X86::None>();
}
atl::shared_ptr<X86::Operand> GenerateX86::visit(ReferenceType &rt) {
  return atl::shared_ptr<X86::None>();
}
atl::shared_ptr<X86::Operand> GenerateX86::visit(Return &r) {
  if (r.returnExpr) {
    atl::shared_ptr<X86::Operand> rVal = r.returnExpr->accept(*this);
    x86.mov(X86::eax, rVal);
  }
  return atl::shared_ptr<X86::None>();
}
atl::shared_ptr<X86::Operand> GenerateX86::visit(SizeOf &so) {
  return atl::shared_ptr<X86::None>();
}
atl::shared_ptr<X86::Operand> GenerateX86::visit(StaticCast &sc) {
  return sc.expr->accept(*this);
}
atl::shared_ptr<X86::Operand> GenerateX86::visit(StringLiteral &sl) {
  return atl::shared_ptr<X86::None>();
}
atl::shared_ptr<X86::Operand> GenerateX86::visit(SubscriptOp &so) {
  return atl::shared_ptr<X86::None>();
}
atl::shared_ptr<X86::Operand> GenerateX86::visit(TemplateDef &td) {
  return atl::shared_ptr<X86::None>();
}
atl::shared_ptr<X86::Operand> GenerateX86::visit(TertiaryExpr &t) {
  return atl::shared_ptr<X86::None>();
}
atl::shared_ptr<X86::Operand> GenerateX86::visit(Throw &t) {
  return atl::shared_ptr<X86::None>();
}
atl::shared_ptr<X86::Operand> GenerateX86::visit(TypeDefDecl &td) {
  return atl::shared_ptr<X86::None>();
}
atl::shared_ptr<X86::Operand> GenerateX86::visit(ValueAt &va) {
  va.derefExpr->accept(*this);
  return atl::shared_ptr<X86::None>();
}
atl::shared_ptr<X86::Operand> GenerateX86::visit(VarDecl &vd) {
  int bytesRequired = vd.getBytes();
  currFpOffset -= bytesRequired;
  vd.fpOffset = currFpOffset;

  const atl::string comment = "Allocated " + atl::to_string(bytesRequired) +
                              "B for VarDecl: " + vd.getIdentifier() +
                              " @ [ebp" + atl::to_string(currFpOffset) + "]";

  x86.sub(X86::esp, bytesRequired, comment);
  return atl::shared_ptr<X86::None>();
}
atl::shared_ptr<X86::Operand> GenerateX86::visit(VarDef &vd) {
  int bytesRequired = vd.getBytes();
  currFpOffset -= bytesRequired;
  vd.fpOffset = currFpOffset;

  const atl::string comment = "Allocated " + atl::to_string(bytesRequired) +
                              "B for VarDef: " + vd.getIdentifier() +
                              " @ [ebp" + atl::to_string(currFpOffset) + "]";

  x86.sub(X86::esp, bytesRequired, comment);
  return atl::shared_ptr<X86::None>();
}
atl::shared_ptr<X86::Operand> GenerateX86::visit(VarExpr &ve) {
  /* Find this Variable's Location in the Stack, and Load It. */
  const int fpOffset = ve.varDecl.lock()->fpOffset;
  if (fpOffset == 0)
    return atl::shared_ptr<X86::GlobalVariable>(new X86::GlobalVariable(
        ve.varDecl.lock()->getIdentifier()->toString(), ve.varDecl.lock()->getBytes()));

  if (fpOffset > 0)
    return atl::shared_ptr<X86::Register>(new 
        X86::Register(0, "[ebp+" + atl::to_string(fpOffset) + "]"));
  else
    return atl::shared_ptr<X86::Register>(new 
        X86::Register(0, "[ebp" + atl::to_string(fpOffset) + "]"));
}
atl::shared_ptr<X86::Operand> GenerateX86::visit(While &w) {
  w.condition->accept(*this);
  w.body->accept(*this);
  return atl::shared_ptr<X86::None>();
}

/* ---- Helpers ---- */

atl::shared_ptr<X86::Operand> GenerateX86::genIntValue(int value) {
  return atl::shared_ptr<X86::IntValue>(new X86::IntValue(atl::to_string(value)));
}