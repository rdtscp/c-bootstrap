#include "ast/ArrayAccess.h"

using namespace ACC;

ArrayAccess::ArrayAccess(const atl::shared_ptr<Expr> &p_array,
                         const atl::shared_ptr<Expr> &p_index)
    : array(p_array), index(p_index) {}

bool ArrayAccess::operator==(Expr &rhs) const {
  if (rhs.astClass() == astClass())
    return *this == *static_cast<ArrayAccess *>(&rhs);
  return false;
}

bool ArrayAccess::operator!=(Expr &rhs) const { return !(*this == rhs); }

bool ArrayAccess::operator==(const ArrayAccess &rhs) const {
  if (*array != *rhs.array)
    return false;

  if (index != rhs.index)
    return false;

  return true;
}

bool ArrayAccess::operator!=(const ArrayAccess &rhs) const {
  return !(*this == rhs);
}