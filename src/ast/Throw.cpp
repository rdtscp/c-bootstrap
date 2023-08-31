#include "ast/Throw.h"

using namespace ACC;

Throw::Throw() : exceptionText(nullptr) {}

Throw::Throw(const atl::shared_ptr<StringLiteral> &p_exceptionText)
    : exceptionText(p_exceptionText) {}

bool Throw::operator==(const Stmt &rhs) const {
  if (rhs.astClass() == astClass())
    return *this == *static_cast<const Throw *>(&rhs);
  return false;
}

bool Throw::operator!=(const Stmt &rhs) const { return !(*this == rhs); }

bool Throw::operator==(const Throw &rhs) const {
  if (*exceptionText != *rhs.exceptionText)
    return false;

  return true;
}

bool Throw::operator!=(const Throw &rhs) const { return !(*this == rhs); }
