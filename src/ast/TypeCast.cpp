#include "ast/TypeCast.h"

using namespace ACC;

TypeCast::TypeCast(const atl::shared_ptr<Type> &p_type,
                   const atl::shared_ptr<Expr> &p_expr)
    : type(p_type), expr(p_expr) {}

bool TypeCast::operator==(Expr &rhs) const {
  if (rhs.astClass() == astClass())
    return *this == *static_cast<TypeCast *>(&rhs);
  return false;
}

bool TypeCast::operator!=(Expr &rhs) const { return !(*this == rhs); }

bool TypeCast::operator==(const TypeCast &rhs) const {
  if (*type != *rhs.type)
    return false;

  if (*expr != *rhs.expr)
    return false;

  return true;
}

bool TypeCast::operator!=(const TypeCast &rhs) const { return !(*this == rhs); }
