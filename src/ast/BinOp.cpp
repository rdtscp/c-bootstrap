#include "ast/BinOp.h"

using namespace ACC;

BinOp::BinOp(const atl::shared_ptr<Expr> &p_lhs, const Op p_operation,
             const atl::shared_ptr<Expr> &p_rhs)
    : lhs(p_lhs), operation(p_operation), rhs(p_rhs) {}

atl::string BinOp::getSignature() const {
  switch (operation) {
  case Op::ADD:
    return lhs->getSignature();
  case Op::SUB:
    return lhs->getSignature();
  case Op::MUL:
    return lhs->getSignature();
  case Op::DIV:
    return lhs->getSignature();
  case Op::MOD:
    return "int";
  case Op::GT:
    return "bool";
  case Op::LT:
    return "bool";
  case Op::GE:
    return "bool";
  case Op::LE:
    return "bool";
  case Op::NE:
    return "bool";
  case Op::EQ:
    return "bool";
  case Op::OR:
    return "bool";
  case Op::AND:
    return "bool";
  case Op::ASSIGNADD:
    return lhs->getSignature();
  default:
    return "int";
  }
}

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
