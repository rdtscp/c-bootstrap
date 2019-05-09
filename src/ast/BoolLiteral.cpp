#include "ast/BoolLiteral.h"

using namespace ACC;

BoolLiteral::BoolLiteral(const atl::string &p_literal) : Literal(p_literal) {}

atl::string BoolLiteral::getSignature() const { return "bool"; }

bool BoolLiteral::operator==(Expr &rhs) const {
  if (rhs.astClass() == astClass())
    return *this == *static_cast<BoolLiteral *>(&rhs);
  return false;
}

bool BoolLiteral::operator!=(Expr &rhs) const { return !(*this == rhs); }

bool BoolLiteral::operator==(Literal &rhs) const {
  if (rhs.astClass() == astClass())
    return *this == *static_cast<BoolLiteral *>(&rhs);
  return false;
}

bool BoolLiteral::operator!=(Literal &rhs) const { return !(*this == rhs); }

bool BoolLiteral::operator==(const BoolLiteral &rhs) const {
  if (value != rhs.value)
    return false;

  return true;
}

bool BoolLiteral::operator!=(const BoolLiteral &rhs) const {
  return !(*this == rhs);
}
