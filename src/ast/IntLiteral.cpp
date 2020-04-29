#include "ast/IntLiteral.h"

using namespace ACC;

IntLiteral::IntLiteral(const atl::string &p_literal)
    : Literal(IntLiteral::removeSigned(p_literal)), isUnsigned(false) {
  if (p_literal[p_literal.size() - 1] == 'u')
    isUnsigned = true;
}

bool IntLiteral::operator==(Expr &rhs) const {
  if (rhs.astClass() == astClass())
    return *this == *static_cast<IntLiteral *>(&rhs);
  return false;
}

bool IntLiteral::operator!=(Expr &rhs) const { return !(*this == rhs); }

bool IntLiteral::operator==(const IntLiteral &rhs) const {
  if (value != rhs.value)
    return false;

  return true;
}

bool IntLiteral::operator!=(const IntLiteral &rhs) const { return !(*this == rhs); }

bool IntLiteral::operator==(Literal &rhs) const {
  if (rhs.astClass() == astClass())
    return *this == *static_cast<IntLiteral *>(&rhs);
  return false;
}

bool IntLiteral::operator!=(Literal &rhs) const { return !(*this == rhs); }

bool IntLiteral::operator==(const Stmt &rhs) const {
  if (rhs.astClass() == astClass())
    return *this == *static_cast<const IntLiteral *>(&rhs);
  return false;
}

bool IntLiteral::operator!=(const Stmt &rhs) const { return !(*this == rhs); }
