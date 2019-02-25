#include "../../include/passes/Optimiser.h"

using namespace ACC;

Optimiser::Optimiser(atl::shared_ptr<Program> &progAST) : progAST(progAST) {}

void Optimiser::optimised(const atl::string &error) {
  optimisationsCount++;
  optimisations.push_back(error);
}

void Optimiser::printOptimisations() {
  for (int idx = 0; idx < optimisations.size(); ++idx)
    printf("\t%s\n", optimisations[idx].c_str());
}

void Optimiser::run() {
  optimisationsCount = 0;
  optimisations.clear();
  visit(*progAST);
}

/* ---- Visit AST ---- */

atl::shared_ptr<ASTNode> Optimiser::visit(ArrayAccess &aa) {
  aa.array = atl::static_pointer_cast<Expr>(aa.array->accept(*this));
  return aa.getptr();
}
atl::shared_ptr<ASTNode> Optimiser::visit(ArrayType &at) { return at.getptr(); }
atl::shared_ptr<ASTNode> Optimiser::visit(Assign &as) {
  as.lhs = atl::static_pointer_cast<Expr>(as.lhs->accept(*this));
  as.rhs = atl::static_pointer_cast<Expr>(as.rhs->accept(*this));
  return as.getptr();
}
atl::shared_ptr<ASTNode> Optimiser::visit(BaseType &bt) { return bt.getptr(); }
atl::shared_ptr<ASTNode> Optimiser::visit(BinOp &bo) {
  /* If we are doing BinOp of two IntLiterals, just do it now. */
  while (bo.lhs->astClass() == "ParenthExpr") {
    bo.lhs = atl::static_pointer_cast<ParenthExpr>(bo.lhs)->innerExpr;
  }
  while (bo.rhs->astClass() == "ParenthExpr") {
    bo.rhs = atl::static_pointer_cast<ParenthExpr>(bo.rhs)->innerExpr;
  }
  if (bo.lhs->astClass() == "IntLiteral" &&
      bo.rhs->astClass() == "IntLiteral") {
    atl::shared_ptr<IntLiteral> lhsIntLiteral =
        atl::static_pointer_cast<IntLiteral>(bo.lhs);
    atl::shared_ptr<IntLiteral> rhsIntLiteral =
        atl::static_pointer_cast<IntLiteral>(bo.rhs);

    int lhsVal = std::stoi(lhsIntLiteral->getLiteral().c_str());
    int rhsVal = std::stoi(rhsIntLiteral->getLiteral().c_str());

    int newVal;
    optimised("Converted a BinOp of Two IntLiterals into its Result.");
    switch (bo.operation) {
    case Op::ADD:
      newVal = lhsVal + rhsVal;
      return atl::make_shared<IntLiteral>(IntLiteral(atl::to_string(newVal)));
    case Op::AND:
      newVal = lhsVal && rhsVal;
      return atl::make_shared<IntLiteral>(IntLiteral(atl::to_string(newVal)));
    case Op::DIV:
      newVal = lhsVal / rhsVal;
      return atl::make_shared<IntLiteral>(IntLiteral(atl::to_string(newVal)));
    case Op::EQ:
      newVal = lhsVal == rhsVal;
      return atl::make_shared<IntLiteral>(IntLiteral(atl::to_string(newVal)));
    case Op::GE:
      newVal = lhsVal >= rhsVal;
      return atl::make_shared<IntLiteral>(IntLiteral(atl::to_string(newVal)));
    case Op::GT:
      newVal = lhsVal > rhsVal;
      return atl::make_shared<IntLiteral>(IntLiteral(atl::to_string(newVal)));
    case Op::LE:
      newVal = lhsVal <= rhsVal;
      return atl::make_shared<IntLiteral>(IntLiteral(atl::to_string(newVal)));
    case Op::LT:
      newVal = lhsVal < rhsVal;
      return atl::make_shared<IntLiteral>(IntLiteral(atl::to_string(newVal)));
    case Op::MOD:
      newVal = lhsVal % rhsVal;
      return atl::make_shared<IntLiteral>(IntLiteral(atl::to_string(newVal)));
    case Op::MUL:
      newVal = lhsVal * rhsVal;
      return atl::make_shared<IntLiteral>(IntLiteral(atl::to_string(newVal)));
    case Op::NE:
      newVal = lhsVal != rhsVal;
      return atl::make_shared<IntLiteral>(IntLiteral(atl::to_string(newVal)));
    case Op::OR:
      newVal = lhsVal || rhsVal;
      return atl::make_shared<IntLiteral>(IntLiteral(atl::to_string(newVal)));
    case Op::SUB:
      newVal = lhsVal - rhsVal;
      return atl::make_shared<IntLiteral>(IntLiteral(atl::to_string(newVal)));
    }
  }
  bo.lhs = atl::static_pointer_cast<Expr>(bo.lhs->accept(*this));
  bo.rhs = atl::static_pointer_cast<Expr>(bo.rhs->accept(*this));
  return bo.getptr();
}
atl::shared_ptr<ASTNode> Optimiser::visit(Block &b) {
  if (b.outerBlock == nullptr) {
    b.setOuterBlock(currScope);
    currScope = b.getptr();
  }
  for (int idx = 0; idx < b.blockStmts.size(); ++idx)
    b.blockStmts[idx] =
        atl::static_pointer_cast<Stmt>(b.blockStmts[idx]->accept(*this));
  currScope = b.outerBlock;
  return b.getptr();
}
atl::shared_ptr<ASTNode> Optimiser::visit(CharLiteral &cl) {
  return cl.getptr();
}
atl::shared_ptr<ASTNode> Optimiser::visit(ClassType &ct) { return ct.getptr(); }
atl::shared_ptr<ASTNode> Optimiser::visit(ClassTypeDecl &ctd) {
  return ctd.getptr();
}
atl::shared_ptr<ASTNode> Optimiser::visit(DoWhile &dw) { return dw.getptr(); }
atl::shared_ptr<ASTNode> Optimiser::visit(EnumTypeDecl &etd) {
  return atl::static_pointer_cast<Decl>(etd.getptr());
}
atl::shared_ptr<ASTNode> Optimiser::visit(FieldAccess &fa) {
  fa.object = atl::static_pointer_cast<Expr>(fa.object->accept(*this));
  return fa.getptr();
}
atl::shared_ptr<ASTNode> Optimiser::visit(FunCall &fc) {
  for (int idx = 0; idx < fc.funArgs.size(); ++idx)
    fc.funArgs[idx] =
        atl::static_pointer_cast<Expr>(fc.funArgs[idx]->accept(*this));
  return fc.getptr();
}
atl::shared_ptr<ASTNode> Optimiser::visit(FunDecl &fd) {
  // fd.funBlock->setOuterBlock(currScope);
  // currScope = fd.funBlock;

  // // for (auto &param : fd.funParams)
  // // param = atl::static_pointer_cast<VarDecl>(param->accept(*this));
  // fd.funBlock = atl::static_pointer_cast<Block>(fd.funBlock->accept(*this));
  // currScope = fd.funBlock->outerBlock;
  return fd.getptr();
}
atl::shared_ptr<ASTNode> Optimiser::visit(FunDef &fd) {
  fd.funBlock->setOuterBlock(currScope);
  currScope = fd.funBlock;

  // for (auto &param : fd.funParams)
  // param = atl::static_pointer_cast<VarDecl>(param->accept(*this));
  fd.funBlock = atl::static_pointer_cast<Block>(fd.funBlock->accept(*this));
  currScope = fd.funBlock->outerBlock;
  return fd.getptr();
}
atl::shared_ptr<ASTNode> Optimiser::visit(If &i) {
  i.ifCondition = atl::static_pointer_cast<Expr>(i.ifCondition->accept(*this));
  i.ifBody = atl::static_pointer_cast<Stmt>(i.ifBody->accept(*this));
  if (i.elseBody)
    i.elseBody = atl::static_pointer_cast<Stmt>(i.elseBody->accept(*this));
  return i.getptr();
}
atl::shared_ptr<ASTNode> Optimiser::visit(IntLiteral &il) {
  return il.getptr();
}
atl::shared_ptr<ASTNode> Optimiser::visit(Namespace &n) {
  for (int i = 0; i < n.namespaceDecls.size(); ++i) {
    n.namespaceDecls[i] =
        atl::static_pointer_cast<Decl>(n.namespaceDecls[i]->accept(*this));
  }
  return n.getptr();
}
atl::shared_ptr<ASTNode> Optimiser::visit(ParenthExpr &pe) {
  pe.innerExpr = atl::static_pointer_cast<Expr>(pe.innerExpr->accept(*this));
  return pe.getptr();
}
atl::shared_ptr<ASTNode> Optimiser::visit(PointerType &pt) {
  return pt.getptr();
}
atl::shared_ptr<ASTNode> Optimiser::visit(Program &p) {
  currScope = atl::make_shared<Block>(Block({}));
  for (int idx = 0; idx < p.decls.size(); ++idx)
    p.decls[idx] = atl::static_pointer_cast<Decl>(p.decls[idx]->accept(*this));

  p.globalScope = currScope;
  return nullptr;
}
atl::shared_ptr<ASTNode> Optimiser::visit(Return &r) {
  if (r.returnExpr)
    r.returnExpr = atl::static_pointer_cast<Expr>(r.returnExpr->accept(*this));
  return r.getptr();
}
atl::shared_ptr<ASTNode> Optimiser::visit(SizeOf &so) { return so.getptr(); }
atl::shared_ptr<ASTNode> Optimiser::visit(StringLiteral &sl) {
  return sl.getptr();
}
atl::shared_ptr<ASTNode> Optimiser::visit(StructType &st) {
  return st.getptr();
}
atl::shared_ptr<ASTNode> Optimiser::visit(StructTypeDecl &std) {
  return std.getptr();
}
atl::shared_ptr<ASTNode> Optimiser::visit(TypeCast &tc) {
  tc.expr = atl::static_pointer_cast<Expr>(tc.expr->accept(*this));
  return tc.getptr();
}
atl::shared_ptr<ASTNode> Optimiser::visit(TypeDefDecl &td) {
  return td.getptr();
}
atl::shared_ptr<ASTNode> Optimiser::visit(ValueAt &va) {
  va.derefExpr = atl::static_pointer_cast<Expr>(va.derefExpr->accept(*this));
  return va.getptr();
}
atl::shared_ptr<ASTNode> Optimiser::visit(VarDecl &vd) {
  return atl::static_pointer_cast<Decl>(vd.getptr());
}
atl::shared_ptr<ASTNode> Optimiser::visit(VarExpr &ve) { return ve.getptr(); }
atl::shared_ptr<ASTNode> Optimiser::visit(While &w) {
  w.condition = atl::static_pointer_cast<Expr>(w.condition->accept(*this));
  w.body = atl::static_pointer_cast<Stmt>(w.body->accept(*this));
  return w.getptr();
}