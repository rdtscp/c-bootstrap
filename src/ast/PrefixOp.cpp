
#include "ast/PrefixOp.h"

using namespace ACC;

PrefixOp::PrefixOp(const Op p_operation,
                   const atl::shared_ptr<VarExpr> &p_variable)
    : operation(p_operation), variable(p_variable) {}

atl::string PrefixOp::getSignature() const { return "int"; }

bool PrefixOp::operator==(Expr &rhs) const {
  if (rhs.astClass() == astClass())
    return *this == *static_cast<PrefixOp *>(&rhs);
  return false;
}

bool PrefixOp::operator!=(Expr &rhs) const { return !(*this == rhs); }

bool PrefixOp::operator==(const PrefixOp &rhs) const {
  if (operation != rhs.operation)
    return false;

  if (*variable != *rhs.variable)
    return false;

  return true;
}

bool PrefixOp::operator!=(const PrefixOp &rhs) const { return !(*this == rhs); }
