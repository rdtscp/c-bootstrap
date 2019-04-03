#include "ast/VarExpr.h"

using namespace ACC;

VarExpr::VarExpr(const atl::shared_ptr<Identifier> &p_varIdentifier)
    : varIdentifier(p_varIdentifier) {}

bool VarExpr::operator==(Expr &rhs) const {
  if (rhs.astClass() == astClass())
    return *this == *static_cast<VarExpr *>(&rhs);
  return false;
}

bool VarExpr::operator!=(Expr &rhs) const { return !(*this == rhs); }

bool VarExpr::operator==(const VarExpr &rhs) const {
  if (*varIdentifier != *rhs.varIdentifier)
    return false;

  return true;
}

bool VarExpr::operator!=(const VarExpr &rhs) const { return !(*this == rhs); }
