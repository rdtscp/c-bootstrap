#include "../../include/passes/Optimiser.h"

#include <memory>

using namespace ACC;

Optimiser::Optimiser(std::shared_ptr<Program> &progAST) : progAST(progAST) {}

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

std::shared_ptr<ASTNode> Optimiser::visit(ArrayAccess &aa) {
  aa.array = std::static_pointer_cast<Expr>(aa.array->accept(*this));
  return aa.getptr();
}
std::shared_ptr<ASTNode> Optimiser::visit(ArrayType &at) { return at.getptr(); }
std::shared_ptr<ASTNode> Optimiser::visit(Assign &as) {
  as.lhs = std::static_pointer_cast<Expr>(as.lhs->accept(*this));
  as.rhs = std::static_pointer_cast<Expr>(as.rhs->accept(*this));
  return as.getptr();
}
std::shared_ptr<ASTNode> Optimiser::visit(BaseType &bt) { return bt.getptr(); }
std::shared_ptr<ASTNode> Optimiser::visit(BinOp &bo) {
  /* If we are doing BinOp of two IntLiterals, just do it now. */
  while (bo.lhs->astClass() == "ParenthExpr") {
    bo.lhs = std::static_pointer_cast<ParenthExpr>(bo.lhs)->innerExpr;
  }
  while (bo.rhs->astClass() == "ParenthExpr") {
    bo.rhs = std::static_pointer_cast<ParenthExpr>(bo.rhs)->innerExpr;
  }
  if (bo.lhs->astClass() == "IntLiteral" &&
      bo.rhs->astClass() == "IntLiteral") {
    std::shared_ptr<IntLiteral> lhsIntLiteral =
        std::static_pointer_cast<IntLiteral>(bo.lhs);
    std::shared_ptr<IntLiteral> rhsIntLiteral =
        std::static_pointer_cast<IntLiteral>(bo.rhs);

    int lhsVal = std::stoi(lhsIntLiteral->getLiteral().c_str());
    int rhsVal = std::stoi(rhsIntLiteral->getLiteral().c_str());

    int newVal;
    optimised("Converted a BinOp of Two IntLiterals into its Result.");
    switch (bo.operation) {
    case Op::ADD:
      newVal = lhsVal + rhsVal;
      return std::make_shared<IntLiteral>(IntLiteral(atl::to_string(newVal)));
    case Op::AND:
      newVal = lhsVal && rhsVal;
      return std::make_shared<IntLiteral>(IntLiteral(atl::to_string(newVal)));
    case Op::DIV:
      newVal = lhsVal / rhsVal;
      return std::make_shared<IntLiteral>(IntLiteral(atl::to_string(newVal)));
    case Op::EQ:
      newVal = lhsVal == rhsVal;
      return std::make_shared<IntLiteral>(IntLiteral(atl::to_string(newVal)));
    case Op::GE:
      newVal = lhsVal >= rhsVal;
      return std::make_shared<IntLiteral>(IntLiteral(atl::to_string(newVal)));
    case Op::GT:
      newVal = lhsVal > rhsVal;
      return std::make_shared<IntLiteral>(IntLiteral(atl::to_string(newVal)));
    case Op::LE:
      newVal = lhsVal <= rhsVal;
      return std::make_shared<IntLiteral>(IntLiteral(atl::to_string(newVal)));
    case Op::LT:
      newVal = lhsVal < rhsVal;
      return std::make_shared<IntLiteral>(IntLiteral(atl::to_string(newVal)));
    case Op::MOD:
      newVal = lhsVal % rhsVal;
      return std::make_shared<IntLiteral>(IntLiteral(atl::to_string(newVal)));
    case Op::MUL:
      newVal = lhsVal * rhsVal;
      return std::make_shared<IntLiteral>(IntLiteral(atl::to_string(newVal)));
    case Op::NE:
      newVal = lhsVal != rhsVal;
      return std::make_shared<IntLiteral>(IntLiteral(atl::to_string(newVal)));
    case Op::OR:
      newVal = lhsVal || rhsVal;
      return std::make_shared<IntLiteral>(IntLiteral(atl::to_string(newVal)));
    case Op::SUB:
      newVal = lhsVal - rhsVal;
      return std::make_shared<IntLiteral>(IntLiteral(atl::to_string(newVal)));
    }
  }
  bo.lhs = std::static_pointer_cast<Expr>(bo.lhs->accept(*this));
  bo.rhs = std::static_pointer_cast<Expr>(bo.rhs->accept(*this));
  return bo.getptr();
}
std::shared_ptr<ASTNode> Optimiser::visit(Block &b) {
  if (b.outerBlock == nullptr) {
    b.setOuterBlock(currScope);
    currScope = b.getptr();
  }
  for (int idx = 0; idx < b.blockStmts.size(); ++idx)
    b.blockStmts[idx] =
        std::static_pointer_cast<Stmt>(b.blockStmts[idx]->accept(*this));
  currScope = b.outerBlock;
  return b.getptr();
}
std::shared_ptr<ASTNode> Optimiser::visit(CharLiteral &cl) {
  return cl.getptr();
}
std::shared_ptr<ASTNode> Optimiser::visit(DoWhile &dw) { return dw.getptr(); }
std::shared_ptr<ASTNode> Optimiser::visit(EnumTypeDecl &etd) {
  return std::static_pointer_cast<Decl>(etd.getptr());
}
std::shared_ptr<ASTNode> Optimiser::visit(FieldAccess &fa) {
  fa.object = std::static_pointer_cast<Expr>(fa.object->accept(*this));
  return fa.getptr();
}
std::shared_ptr<ASTNode> Optimiser::visit(FunCall &fc) {
  for (int idx = 0; idx < fc.funArgs.size(); ++idx)
    fc.funArgs[idx] =
        std::static_pointer_cast<Expr>(fc.funArgs[idx]->accept(*this));
  return fc.getptr();
}
std::shared_ptr<ASTNode> Optimiser::visit(FunDecl &fd) {
  // fd.funBlock->setOuterBlock(currScope);
  // currScope = fd.funBlock;

  // // for (auto &param : fd.funParams)
  // // param = std::static_pointer_cast<VarDecl>(param->accept(*this));
  // fd.funBlock = std::static_pointer_cast<Block>(fd.funBlock->accept(*this));
  // currScope = fd.funBlock->outerBlock;
  return fd.getptr();
}
std::shared_ptr<ASTNode> Optimiser::visit(FunDef &fd) {
  fd.funBlock->setOuterBlock(currScope);
  currScope = fd.funBlock;

  // for (auto &param : fd.funParams)
  // param = std::static_pointer_cast<VarDecl>(param->accept(*this));
  fd.funBlock = std::static_pointer_cast<Block>(fd.funBlock->accept(*this));
  currScope = fd.funBlock->outerBlock;
  return fd.getptr();
}
std::shared_ptr<ASTNode> Optimiser::visit(If &i) {
  i.ifCondition = std::static_pointer_cast<Expr>(i.ifCondition->accept(*this));
  i.ifBody = std::static_pointer_cast<Stmt>(i.ifBody->accept(*this));
  if (i.elseBody)
    i.elseBody = std::static_pointer_cast<Stmt>(i.elseBody->accept(*this));
  return i.getptr();
}
std::shared_ptr<ASTNode> Optimiser::visit(IntLiteral &il) {
  return il.getptr();
}
std::shared_ptr<ASTNode> Optimiser::visit(Namespace &n) {
  n.namespaceBlock =
      std::static_pointer_cast<Block>(n.namespaceBlock->accept(*this));
  return n.getptr();
}
std::shared_ptr<ASTNode> Optimiser::visit(ParenthExpr &pe) {
  pe.innerExpr = std::static_pointer_cast<Expr>(pe.innerExpr->accept(*this));
  return pe.getptr();
}
std::shared_ptr<ASTNode> Optimiser::visit(PointerType &pt) {
  return pt.getptr();
}
std::shared_ptr<ASTNode> Optimiser::visit(Program &p) {
  currScope = std::make_shared<Block>(Block({}));
  for (int idx = 0; idx < p.decls.size(); ++idx)
    p.decls[idx] = std::static_pointer_cast<Decl>(p.decls[idx]->accept(*this));

  p.globalScope = currScope;
  return nullptr;
}
std::shared_ptr<ASTNode> Optimiser::visit(Return &r) {
  if (r.returnExpr)
    r.returnExpr = std::static_pointer_cast<Expr>(r.returnExpr->accept(*this));
  return r.getptr();
}
std::shared_ptr<ASTNode> Optimiser::visit(SizeOf &so) { return so.getptr(); }
std::shared_ptr<ASTNode> Optimiser::visit(StringLiteral &sl) {
  return sl.getptr();
}
std::shared_ptr<ASTNode> Optimiser::visit(StructType &st) {
  return st.getptr();
}
std::shared_ptr<ASTNode> Optimiser::visit(StructTypeDecl &std) {
  return std.getptr();
}
std::shared_ptr<ASTNode> Optimiser::visit(TypeCast &tc) {
  tc.expr = std::static_pointer_cast<Expr>(tc.expr->accept(*this));
  return tc.getptr();
}
std::shared_ptr<ASTNode> Optimiser::visit(TypeDefDecl &td) {
  return td.getptr();
}
std::shared_ptr<ASTNode> Optimiser::visit(ValueAt &va) {
  va.derefExpr = std::static_pointer_cast<Expr>(va.derefExpr->accept(*this));
  return va.getptr();
}
std::shared_ptr<ASTNode> Optimiser::visit(VarDecl &vd) {
  return std::static_pointer_cast<Decl>(vd.getptr());
}
std::shared_ptr<ASTNode> Optimiser::visit(VarExpr &ve) { return ve.getptr(); }
std::shared_ptr<ASTNode> Optimiser::visit(While &w) {
  w.condition = std::static_pointer_cast<Expr>(w.condition->accept(*this));
  w.body = std::static_pointer_cast<Stmt>(w.body->accept(*this));
  return w.getptr();
}