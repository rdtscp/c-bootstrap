#include "ast/BinOp.h"

using namespace ACC;

BinOp::BinOp(const atl::shared_ptr<Expr> &p_lhs, const Op p_operation,
             const atl::shared_ptr<Expr> &p_rhs)
    : lhs(p_lhs), operation(p_operation), rhs(p_rhs) {}

bool BinOp::operator==(Expr &rhs) const {
  if (rhs.astClass() == astClass())
    return *this == *static_cast<BinOp *>(&rhs);
  return false;
}

bool BinOp::operator!=(Expr &rhs) const { return !(*this == rhs); }

bool BinOp::operator==(const BinOp &other) const {
  if (*lhs != *other.lhs)
    return false;

  if (operation != other.operation)
    return false;

  if (rhs != other.rhs)
    return false;

  return true;
}

bool BinOp::operator!=(const BinOp &rhs) const { return !(*this == rhs); }
