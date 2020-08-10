#include "passes/Optimiser.h"

using namespace ACC;

Optimiser::Optimiser(atl::shared_ptr<Program> &progAST) : progAST(progAST) {}

void Optimiser::optimised(const atl::string &message,
                          const atl::shared_ptr<ASTNode> &node) {
  optimisationsCount++;
  optimisations.push_back("Optimisation at: " + node->position.toString() +
                          "\n\t" + message + "\n\t\t");
}

void Optimiser::printOptimisations() {
  for (unsigned int idx = 0; idx < optimisations.size(); ++idx)
    printf("\t%s\n", optimisations[idx].c_str());
}

void Optimiser::run() {
  optimisationsCount = 0;
  optimisations.clear();
  visit(*progAST);
}

/* ---- Visit AST ---- */

atl::shared_ptr<ASTNode> Optimiser::visit(AddressOf &ao) { return ao.getptr(); }
atl::shared_ptr<ASTNode> Optimiser::visit(Allocation &a) { return a.getptr(); }
atl::shared_ptr<ASTNode> Optimiser::visit(ArrayType &at) { return at.getptr(); }
atl::shared_ptr<ASTNode> Optimiser::visit(Assign &as) {
  as.lhs = as.lhs->accept(*this);
  as.rhs = as.rhs->accept(*this);
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

    int lhsVal = atl::stoi(lhsIntLiteral->getLiteral());
    int rhsVal = atl::stoi(rhsIntLiteral->getLiteral());

    int newVal;
    optimised("Converted a BinOp of Two IntLiterals into its Result.",
              bo.getptr());
    switch (bo.operation) {
    case Op::ADD:
      newVal = lhsVal + rhsVal;
      return atl::shared_ptr<IntLiteral>(new IntLiteral(atl::to_string(newVal)));
    case Op::AND:
      newVal = lhsVal && rhsVal;
      return atl::shared_ptr<IntLiteral>(new IntLiteral(atl::to_string(newVal)));
    case Op::DIV:
      newVal = lhsVal / rhsVal;
      return atl::shared_ptr<IntLiteral>(new IntLiteral(atl::to_string(newVal)));
    case Op::EQ:
      newVal = lhsVal == rhsVal;
      return atl::shared_ptr<IntLiteral>(new IntLiteral(atl::to_string(newVal)));
    case Op::GE:
      newVal = lhsVal >= rhsVal;
      return atl::shared_ptr<IntLiteral>(new IntLiteral(atl::to_string(newVal)));
    case Op::GT:
      newVal = lhsVal > rhsVal;
      return atl::shared_ptr<IntLiteral>(new IntLiteral(atl::to_string(newVal)));
    case Op::LE:
      newVal = lhsVal <= rhsVal;
      return atl::shared_ptr<IntLiteral>(new IntLiteral(atl::to_string(newVal)));
    case Op::LT:
      newVal = lhsVal < rhsVal;
      return atl::shared_ptr<IntLiteral>(new IntLiteral(atl::to_string(newVal)));
    case Op::MOD:
      newVal = lhsVal % rhsVal;
      return atl::shared_ptr<IntLiteral>(new IntLiteral(atl::to_string(newVal)));
    case Op::MUL:
      newVal = lhsVal * rhsVal;
      return atl::shared_ptr<IntLiteral>(new IntLiteral(atl::to_string(newVal)));
    case Op::NE:
      newVal = lhsVal != rhsVal;
      return atl::shared_ptr<IntLiteral>(new IntLiteral(atl::to_string(newVal)));
    case Op::OR:
      newVal = lhsVal || rhsVal;
      return atl::shared_ptr<IntLiteral>(new IntLiteral(atl::to_string(newVal)));
    case Op::SUB:
      newVal = lhsVal - rhsVal;
      return atl::shared_ptr<IntLiteral>(new IntLiteral(atl::to_string(newVal)));
    case Op::ASSIGNADD:
      return bo.getptr();
    }
  }
  bo.lhs = atl::static_pointer_cast<Expr>(bo.lhs->accept(*this));
  bo.rhs = atl::static_pointer_cast<Expr>(bo.rhs->accept(*this));
  return bo.getptr();
}
atl::shared_ptr<ASTNode> Optimiser::visit(Block &b) {
  if (b.outerScope.lock() == nullptr) {
    b.outerScope = currScope;
    currScope = b.getptr();
  }
  for (unsigned int idx = 0; idx < b.stmts.size(); ++idx)
    b.stmts[idx] = atl::static_pointer_cast<Stmt>(b.stmts[idx]->accept(*this));
  currScope = b.outerScope.lock();
  return b.getptr();
}
atl::shared_ptr<ASTNode> Optimiser::visit(BoolLiteral &bl) {
  return bl.getptr();
}
atl::shared_ptr<ASTNode> Optimiser::visit(CharLiteral &cl) {
  return cl.getptr();
}
atl::shared_ptr<ASTNode> Optimiser::visit(ClassType &ct) { return ct.getptr(); }
atl::shared_ptr<ASTNode> Optimiser::visit(ClassTypeDecl &ctd) {
  return ctd.getptr();
}
atl::shared_ptr<ASTNode> Optimiser::visit(ClassTypeDef &ctd) {
  return ctd.getptr();
}
atl::shared_ptr<ASTNode> Optimiser::visit(ConstructorCall &cc) {
  for (unsigned int idx = 0; idx < cc.constructorArgs.size(); ++idx)
    cc.constructorArgs[idx] =
        atl::static_pointer_cast<Expr>(cc.constructorArgs[idx]->accept(*this));
  return cc.getptr();
}
atl::shared_ptr<ASTNode> Optimiser::visit(ConstructorDecl &cd) {
  return cd.getptr();
}
atl::shared_ptr<ASTNode> Optimiser::visit(ConstructorDef &cd) {
  return cd.getptr();
}
atl::shared_ptr<ASTNode> Optimiser::visit(Deletion &d) { return d.getptr(); }
atl::shared_ptr<ASTNode> Optimiser::visit(DestructorDecl &dd) {
  return dd.getptr();
}
atl::shared_ptr<ASTNode> Optimiser::visit(DestructorDef &dd) {
  return dd.getptr();
}
atl::shared_ptr<ASTNode> Optimiser::visit(DoWhile &dw) { return dw.getptr(); }
atl::shared_ptr<ASTNode> Optimiser::visit(EnumClassTypeDecl &ectd) {
  return atl::static_pointer_cast<Decl>(ectd.getptr());
}
atl::shared_ptr<ASTNode> Optimiser::visit(For &f) { return f.getptr(); }
atl::shared_ptr<ASTNode> Optimiser::visit(FunCall &fc) {
  for (unsigned int idx = 0; idx < fc.funArgs.size(); ++idx)
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
  fd.funBlock->outerScope = currScope;
  currScope = fd.funBlock;

  // for (auto &param : fd.funParams)
  // param = atl::static_pointer_cast<VarDecl>(param->accept(*this));
  fd.funBlock = atl::static_pointer_cast<Block>(fd.funBlock->accept(*this));
  currScope = fd.funBlock->outerScope.lock();
  return fd.getptr();
}
atl::shared_ptr<ASTNode> Optimiser::visit(Identifier &i) { return i.getptr(); }
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
atl::shared_ptr<ASTNode> Optimiser::visit(MemberAccess &ma) {
  return ma.getptr();
}
atl::shared_ptr<ASTNode> Optimiser::visit(MemberCall &mc) {
  return mc.getptr();
}
atl::shared_ptr<ASTNode> Optimiser::visit(Namespace &n) {
  for (unsigned int i = 0; i < n.namespaceDecls.size(); ++i) {
    n.namespaceDecls[i] =
        atl::static_pointer_cast<Decl>(n.namespaceDecls[i]->accept(*this));
  }
  return n.getptr();
}
atl::shared_ptr<ASTNode> Optimiser::visit(Not &n) {
  n.expr = atl::static_pointer_cast<Expr>(n.expr->accept(*this));
  return n.getptr();
}
atl::shared_ptr<ASTNode> Optimiser::visit(Nullptr &n) { return n.getptr(); }
atl::shared_ptr<ASTNode> Optimiser::visit(ParenthExpr &pe) {
  pe.innerExpr = atl::static_pointer_cast<Expr>(pe.innerExpr->accept(*this));
  return pe.getptr();
}
atl::shared_ptr<ASTNode> Optimiser::visit(PointerType &pt) {
  return pt.getptr();
}
atl::shared_ptr<ASTNode> Optimiser::visit(PrefixOp &po) { return po.getptr(); }
atl::shared_ptr<ASTNode> Optimiser::visit(Program &p) {
  currScope = p.getptr();
  for (unsigned int idx = 0; idx < p.decls.size(); ++idx)
    p.decls[idx] = atl::static_pointer_cast<Decl>(p.decls[idx]->accept(*this));

  return nullptr;
}
atl::shared_ptr<ASTNode> Optimiser::visit(ReferenceType &rt) {
  return rt.getptr();
}
atl::shared_ptr<ASTNode> Optimiser::visit(Return &r) {
  if (r.returnExpr)
    r.returnExpr = atl::static_pointer_cast<Expr>(r.returnExpr->accept(*this));
  return r.getptr();
}
atl::shared_ptr<ASTNode> Optimiser::visit(SizeOf &so) { return so.getptr(); }
atl::shared_ptr<ASTNode> Optimiser::visit(StaticCast &sc) {
  sc.expr = atl::static_pointer_cast<Expr>(sc.expr->accept(*this));
  return sc.getptr();
}
atl::shared_ptr<ASTNode> Optimiser::visit(StringLiteral &sl) {
  return sl.getptr();
}
atl::shared_ptr<ASTNode> Optimiser::visit(SubscriptOp &so) {
  so.variable = so.variable->accept(*this);
  so.index = so.index->accept(*this);
  return so.getptr();
}
atl::shared_ptr<ASTNode> Optimiser::visit(TemplateDef &td) {
  return td.getptr();
}
atl::shared_ptr<ASTNode> Optimiser::visit(TertiaryExpr &t) {
  return t.getptr();
}
atl::shared_ptr<ASTNode> Optimiser::visit(Throw &t) { return t.getptr(); }
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
atl::shared_ptr<ASTNode> Optimiser::visit(VarDef &vd) {
  return atl::static_pointer_cast<Decl>(vd.getptr());
}
atl::shared_ptr<ASTNode> Optimiser::visit(VarExpr &ve) { return ve.getptr(); }
atl::shared_ptr<ASTNode> Optimiser::visit(While &w) {
  w.condition = atl::static_pointer_cast<Expr>(w.condition->accept(*this));
  w.body = atl::static_pointer_cast<Stmt>(w.body->accept(*this));
  return w.getptr();
}