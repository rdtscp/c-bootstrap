#include "../../include/ast/ParenthExpr.h"

using namespace ACC;

ParenthExpr::ParenthExpr(std::shared_ptr<Expr> innerExpr)
    : innerExpr(innerExpr) {}

std::shared_ptr<ParenthExpr> ParenthExpr::getptr() {
  return shared_from_this();
}

void ParenthExpr::accept(ASTVisitor<void> &v) { return v.visit(*this); }

atl::string ParenthExpr::accept(ASTVisitor<atl::string> &v) {
  return v.visit(*this);
}

std::shared_ptr<Type>
ParenthExpr::accept(ASTVisitor<std::shared_ptr<Type>> &v) {
  return v.visit(*this);
}

MIPS::Register ParenthExpr::accept(ASTVisitor<MIPS::Register> &v) {
  return v.visit(*this);
}

std::shared_ptr<X86::Operand>
ParenthExpr::accept(ASTVisitor<std::shared_ptr<X86::Operand>> &v) {
  return v.visit(*this);
}

std::shared_ptr<ASTNode>
ParenthExpr::accept(ASTVisitor<std::shared_ptr<ASTNode>> &v) {
  return v.visit(*this);
}

atl::string ParenthExpr::astClass() const { return "ParenthExpr"; }
