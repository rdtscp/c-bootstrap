#include "ast/Return.h"

using namespace ACC;

Return::Return() : returnExpr(nullptr) {}

Return::Return(const atl::shared_ptr<Expr> &p_returnExpr)
    : returnExpr(p_returnExpr) {}

bool Return::operator==(const Return &rhs) const {
  if (*returnExpr != *rhs.returnExpr)
    return false;

  return true;
}

bool Return::operator!=(const Return &rhs) const { return !(*this == rhs); }

bool Return::operator==(const Stmt &rhs) const {
  if (rhs.astClass() == astClass())
    return *this == *static_cast<const Return *>(&rhs);
  return false;
}

bool Return::operator!=(const Stmt &rhs) const { return !(*this == rhs); }
