#include "ast/Assign.h"

using namespace ACC;

Assign::Assign(const atl::shared_ptr<Expr> &p_lhs,
               const atl::shared_ptr<Expr> &p_rhs)
    : lhs(p_lhs), rhs(p_rhs) {}

bool Assign::operator==(const Assign &other) const {
  if (*lhs != *other.lhs)
    return false;

  if (*rhs != *other.rhs)
    return false;

  return true;
}

bool Assign::operator!=(const Assign &rhs) const { return !(*this == rhs); }

bool Assign::operator==(const Stmt &rhs) const {
  if (rhs.astClass() == astClass())
    return *this == *static_cast<const Assign *>(&rhs);
  return false;
}

bool Assign::operator!=(const Stmt &rhs) const { return !(*this == rhs); }
