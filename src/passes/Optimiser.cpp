#include "../../include/passes/Optimiser.h"

using namespace ACC;

Optimiser::Optimiser(std::shared_ptr<Program> progAST) : progAST(progAST) {}

void Optimiser::optimised(std::string error) {
  optimisationsCount++;
  optimisations.push_back(error);
}

void Optimiser::printOptimisations() {
  std::cerr << "Optimisations Made:" << std::endl;
  for (const auto &optimisation : optimisations)
    std::cerr << "\t" << optimisation << std::endl;
}

void Optimiser::run() { visit(*progAST); }

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
  if (bo.lhs->astClass() == "IntLiteral" &&
      bo.rhs->astClass() == "IntLiteral") {
    std::shared_ptr<IntLiteral> lhsIntLiteral =
        std::static_pointer_cast<IntLiteral>(bo.lhs);
    std::shared_ptr<IntLiteral> rhsIntLiteral =
        std::static_pointer_cast<IntLiteral>(bo.rhs);

    int lhsVal = std::stoi(lhsIntLiteral->value);
    int rhsVal = std::stoi(rhsIntLiteral->value);

    int newVal;
    switch (bo.operation) {
    case Op::ADD:
      newVal = lhsVal + rhsVal;
      return std::make_shared<IntLiteral>(IntLiteral(std::to_string(newVal)));
    default:
      break;
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
  for (auto &stmt : b.blockStmts)
    stmt = std::static_pointer_cast<Stmt>(stmt->accept(*this));
  currScope = b.outerBlock;
  return b.getptr();
}
std::shared_ptr<ASTNode> Optimiser::visit(CharLiteral &cl) {
  return cl.getptr();
}
std::shared_ptr<ASTNode> Optimiser::visit(FieldAccess &fa) {
  fa.object = std::static_pointer_cast<Expr>(fa.object->accept(*this));
  return fa.getptr();
}
std::shared_ptr<ASTNode> Optimiser::visit(FunCall &fc) {
  for (auto &arg : fc.funArgs)
    arg = std::static_pointer_cast<Expr>(arg->accept(*this));
  return fc.getptr();
}
std::shared_ptr<ASTNode> Optimiser::visit(FunDecl &fd) {
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
std::shared_ptr<ASTNode> Optimiser::visit(ParenthExpr &pe) {
  pe.innerExpr = std::static_pointer_cast<Expr>(pe.innerExpr->accept(*this));
  return pe.getptr();
}
std::shared_ptr<ASTNode> Optimiser::visit(PointerType &pt) {
  return pt.getptr();
}
std::shared_ptr<ASTNode> Optimiser::visit(Program &p) {
  currScope = std::make_shared<Block>(Block({}));
  for (std::shared_ptr<Decl> &decl : p.decls) {
    decl = std::static_pointer_cast<Decl>(decl->accept(*this));
  }
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
std::shared_ptr<ASTNode> Optimiser::visit(ValueAt &va) {
  va.derefExpr = std::static_pointer_cast<Expr>(va.derefExpr->accept(*this));
  return va.getptr();
}
std::shared_ptr<ASTNode> Optimiser::visit(VarDecl &vd) { return vd.getptr(); }
std::shared_ptr<ASTNode> Optimiser::visit(VarExpr &ve) { return ve.getptr(); }
std::shared_ptr<ASTNode> Optimiser::visit(While &w) {
  w.condition = std::static_pointer_cast<Expr>(w.condition->accept(*this));
  w.body = std::static_pointer_cast<Stmt>(w.body->accept(*this));
  return w.getptr();
}