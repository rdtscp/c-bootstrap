#include "../../include/ast/ParenthExpr.h"

using namespace ACC;

ParenthExpr::ParenthExpr(atl::shared_ptr<Expr> innerExpr)
    : innerExpr(innerExpr) {}

atl::shared_ptr<ParenthExpr> ParenthExpr::getptr() {
  return shared_from_this();
}

bool ParenthExpr::operator==(Expr &rhs) const {
  if (rhs.astClass() == astClass())
    return *this == *static_cast<ParenthExpr *>(&rhs);
  return false;
}

bool ParenthExpr::operator!=(Expr &rhs) const { return !(*this == rhs); }

bool ParenthExpr::operator==(const ParenthExpr &rhs) const {
  if (*innerExpr != *rhs.innerExpr)
    return false;

  return true;
}

bool ParenthExpr::operator!=(const ParenthExpr &rhs) const {
  return !(*this == rhs);
}

void ParenthExpr::accept(ASTVisitor<void> &v) { return v.visit(*this); }

atl::string ParenthExpr::accept(ASTVisitor<atl::string> &v) {
  return v.visit(*this);
}

atl::shared_ptr<Type>
ParenthExpr::accept(ASTVisitor<atl::shared_ptr<Type>> &v) {
  return v.visit(*this);
}

MIPS::Register ParenthExpr::accept(ASTVisitor<MIPS::Register> &v) {
  return v.visit(*this);
}

atl::shared_ptr<X86::Operand>
ParenthExpr::accept(ASTVisitor<atl::shared_ptr<X86::Operand>> &v) {
  return v.visit(*this);
}

atl::shared_ptr<ASTNode>
ParenthExpr::accept(ASTVisitor<atl::shared_ptr<ASTNode>> &v) {
  return v.visit(*this);
}

atl::string ParenthExpr::astClass() const { return "ParenthExpr"; }
