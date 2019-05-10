#include "ast/ValueAt.h"

using namespace ACC;

ValueAt::ValueAt(const atl::shared_ptr<Expr> &p_derefExpr)
    : derefExpr(p_derefExpr) {}

atl::string ValueAt::getSignature() const {
  const atl::string derefExprSig = derefExpr->getSignature();
  atl::string output;
  for (int idx = 0; idx < derefExprSig.length() - 1; ++idx)
    output += derefExprSig[idx];

  return output;
}

bool ValueAt::operator==(Expr &rhs) const {
  if (rhs.astClass() == astClass())
    return *this == *static_cast<ValueAt *>(&rhs);
  return false;
}

bool ValueAt::operator!=(Expr &rhs) const { return !(*this == rhs); }

bool ValueAt::operator==(const ValueAt &rhs) const {
  if (*derefExpr != *rhs.derefExpr)
    return false;

  return true;
}

bool ValueAt::operator!=(const ValueAt &rhs) const { return !(*this == rhs); }
