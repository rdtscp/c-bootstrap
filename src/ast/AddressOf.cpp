#include "ast/AddressOf.h"

using namespace ACC;

AddressOf::AddressOf(const atl::shared_ptr<Expr> &p_addressOfExpr)
    : addressOfExpr(p_addressOfExpr) {}

bool AddressOf::operator==(const AddressOf &rhs) const {
  if (*addressOfExpr != *rhs.addressOfExpr)
    return false;

  return true;
}

bool AddressOf::operator!=(const AddressOf &rhs) const { return !(*this == rhs); }

bool AddressOf::operator==(Expr &rhs) const {
  if (rhs.astClass() == astClass())
    return *this == *static_cast<AddressOf *>(&rhs);
  return false;
}

bool AddressOf::operator!=(Expr &rhs) const { return !(*this == rhs); }

bool AddressOf::operator==(const Stmt &rhs) const {
  if (rhs.astClass() == astClass())
    return *this == *static_cast<const Stmt *>(&rhs);
  return false;
}

bool AddressOf::operator!=(const Stmt &rhs) const { return !(*this == rhs); }
