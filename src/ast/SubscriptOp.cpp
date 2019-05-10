#include "ast/SubscriptOp.h"

using namespace ACC;

SubscriptOp::SubscriptOp(const atl::shared_ptr<VarExpr> &p_variable,
                         const atl::shared_ptr<Expr> &p_index)
    : variable(p_variable), index(p_index), operatorDef(nullptr) {}

atl::string SubscriptOp::getSignature() const { return "nullptr_t"; }

bool SubscriptOp::operator==(Expr &rhs) const {
  if (rhs.astClass() == astClass())
    return *this == *static_cast<SubscriptOp *>(&rhs);
  return false;
}

bool SubscriptOp::operator!=(Expr &rhs) const { return !(*this == rhs); }

bool SubscriptOp::operator==(const SubscriptOp &rhs) const {
  if (*index != *rhs.index)
    return false;

  if (*variable != *rhs.variable)
    return false;

  return true;
}

bool SubscriptOp::operator!=(const SubscriptOp &rhs) const {
  return !(*this == rhs);
}
