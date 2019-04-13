#include "ast/IntLiteral.h"

using namespace ACC;

IntLiteral::IntLiteral(const atl::string &p_literal) : Literal(p_literal) {}

bool IntLiteral::operator==(Expr &rhs) const {
  if (rhs.astClass() == astClass())
    return *this == *static_cast<IntLiteral *>(&rhs);
  return false;
}

bool IntLiteral::operator!=(Expr &rhs) const { return !(*this == rhs); }

bool IntLiteral::operator==(Literal &rhs) const {
  if (rhs.astClass() == astClass())
    return *this == *static_cast<IntLiteral *>(&rhs);
  return false;
}

bool IntLiteral::operator!=(Literal &rhs) const { return !(*this == rhs); }

bool IntLiteral::operator==(const IntLiteral &rhs) const {
  if (value != rhs.value)
    return false;

  return true;
}

bool IntLiteral::operator!=(const IntLiteral &rhs) const {
  return !(*this == rhs);
}
