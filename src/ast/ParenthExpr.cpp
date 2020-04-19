#include "ast/ParenthExpr.h"

using namespace ACC;

ParenthExpr::ParenthExpr(const atl::shared_ptr<Expr> &p_innerExpr)
    : innerExpr(p_innerExpr) {}

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

bool ParenthExpr::operator==(const Stmt &rhs) const {
  if (rhs.astClass() == astClass())
    return *this == *static_cast<const ParenthExpr *>(&rhs);
  return false;
}

bool ParenthExpr::operator!=(const Stmt &rhs) const { return !(*this == rhs); }
