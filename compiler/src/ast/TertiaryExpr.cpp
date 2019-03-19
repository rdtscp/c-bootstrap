#include "../../include/ast/TertiaryExpr.h"

using namespace ACC;

TertiaryExpr::TertiaryExpr(const atl::shared_ptr<Expr> &p_tertiaryCondition,
                           const atl::shared_ptr<Expr> &p_tertiaryIfBody,
                           const atl::shared_ptr<Expr> &p_tertiaryElseBody)
    : tertiaryCondition(p_tertiaryCondition), tertiaryIfBody(p_tertiaryIfBody),
      tertiaryElseBody(p_tertiaryElseBody) {}

atl::shared_ptr<TertiaryExpr> TertiaryExpr::getptr() {
  return shared_from_this();
}

bool TertiaryExpr::operator==(Expr &rhs) const {
  if (rhs.astClass() == astClass())
    return *this == *static_cast<TertiaryExpr *>(&rhs);
  return false;
}

bool TertiaryExpr::operator!=(Expr &rhs) const { return !(*this == rhs); }

bool TertiaryExpr::operator==(const TertiaryExpr &rhs) const {
  if (*tertiaryCondition != *rhs.tertiaryCondition)
    return false;

  if (*tertiaryIfBody != *rhs.tertiaryIfBody)
    return false;

  if (*tertiaryElseBody != *rhs.tertiaryElseBody)
    return false;

  return true;
}

bool TertiaryExpr::operator!=(const TertiaryExpr &rhs) const {
  return !(*this == rhs);
}

void TertiaryExpr::accept(ASTVisitor<void> &v) { return v.visit(*this); }

atl::string TertiaryExpr::accept(ASTVisitor<atl::string> &v) {
  return v.visit(*this);
}

atl::shared_ptr<Type>
TertiaryExpr::accept(ASTVisitor<atl::shared_ptr<Type>> &v) {
  return v.visit(*this);
}

MIPS::Register TertiaryExpr::accept(ASTVisitor<MIPS::Register> &v) {
  return v.visit(*this);
}

atl::shared_ptr<X86::Operand>
TertiaryExpr::accept(ASTVisitor<atl::shared_ptr<X86::Operand>> &v) {
  return v.visit(*this);
}

atl::shared_ptr<ASTNode>
TertiaryExpr::accept(ASTVisitor<atl::shared_ptr<ASTNode>> &v) {
  return v.visit(*this);
}

atl::string TertiaryExpr::astClass() const { return "TertiaryExpr"; }