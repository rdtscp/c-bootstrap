#include "ast/Not.h"

using namespace ACC;

Not::Not(const atl::shared_ptr<Expr> &p_expr)
    : expr(p_expr) {}

bool Not::operator==(Expr &rhs) const {
  if (rhs.astClass() == astClass())
    return *this == *static_cast<Not *>(&rhs);
  return false;
}

bool Not::operator!=(Expr &rhs) const { return !(*this == rhs); }

bool Not::operator==(const Not &rhs) const {
  if (*expr != *rhs.expr)
    return false;

  return true;
}

bool Not::operator!=(const Not &rhs) const {
  return !(*this == rhs);
}

bool Not::operator==(const Stmt &rhs) const {
  if (rhs.astClass() == astClass())
    return *this == *static_cast<const Not *>(&rhs);
  return false;
}

bool Not::operator!=(const Stmt &rhs) const { return !(*this == rhs); }
