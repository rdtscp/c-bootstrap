#include "ast/CharLiteral.h"

using namespace ACC;

CharLiteral::CharLiteral(const atl::string &p_literal) : Literal(p_literal) {}

bool CharLiteral::operator==(Expr &rhs) const {
  if (rhs.astClass() == astClass())
    return *this == *static_cast<CharLiteral *>(&rhs);
  return false;
}

bool CharLiteral::operator!=(Expr &rhs) const { return !(*this == rhs); }

bool CharLiteral::operator==(Literal &rhs) const {
  if (rhs.astClass() == astClass())
    return *this == *static_cast<CharLiteral *>(&rhs);
  return false;
}

bool CharLiteral::operator!=(Literal &rhs) const { return !(*this == rhs); }

bool CharLiteral::operator==(const CharLiteral &rhs) const {
  if (value != rhs.value)
    return false;

  return true;
}

bool CharLiteral::operator!=(const CharLiteral &rhs) const {
  return !(*this == rhs);
}
