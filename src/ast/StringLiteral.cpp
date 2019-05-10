#include "ast/StringLiteral.h"

using namespace ACC;

StringLiteral::StringLiteral(const atl::string &p_literal)
    : Literal(p_literal) {}

atl::string StringLiteral::getSignature() const {
  return "char*";
}

bool StringLiteral::operator==(Expr &rhs) const {
  if (rhs.astClass() == astClass())
    return *this == *static_cast<StringLiteral *>(&rhs);
  return false;
}

bool StringLiteral::operator!=(Expr &rhs) const { return !(*this == rhs); }

bool StringLiteral::operator==(Literal &rhs) const {
  if (rhs.astClass() == astClass())
    return *this == *static_cast<StringLiteral *>(&rhs);
  return false;
}

bool StringLiteral::operator!=(Literal &rhs) const { return !(*this == rhs); }

bool StringLiteral::operator==(const StringLiteral &rhs) const {
  if (value != rhs.value)
    return false;

  return true;
}

bool StringLiteral::operator!=(const StringLiteral &rhs) const {
  return !(*this == rhs);
}
