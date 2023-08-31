#include "ast/StaticCast.h"

using namespace ACC;

StaticCast::StaticCast(const atl::shared_ptr<Type> &p_type,
                   const atl::shared_ptr<Expr> &p_expr)
    : type(p_type), expr(p_expr) {}

bool StaticCast::operator==(Expr &rhs) const {
  if (rhs.astClass() == astClass())
    return *this == *static_cast<StaticCast *>(&rhs);
  return false;
}

bool StaticCast::operator!=(Expr &rhs) const { return !(*this == rhs); }

bool StaticCast::operator==(const StaticCast &rhs) const {
  if (*type != *rhs.type)
    return false;

  if (*expr != *rhs.expr)
    return false;

  return true;
}

bool StaticCast::operator!=(const StaticCast &rhs) const { return !(*this == rhs); }

bool StaticCast::operator==(const Stmt &rhs) const {
  if (rhs.astClass() == astClass())
    return *this == *static_cast<const StaticCast *>(&rhs);
  return false;
}

bool StaticCast::operator!=(const Stmt &rhs) const { return !(*this == rhs); }
