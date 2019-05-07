#include "ast/Nullptr.h"

using namespace ACC;

Nullptr::Nullptr() {}

bool Nullptr::operator==(Expr &rhs) const {
  if (rhs.astClass() == astClass())
    return *this == *static_cast<Nullptr *>(&rhs);
  return false;
}

bool Nullptr::operator!=(Expr &rhs) const { return !(*this == rhs); }

bool Nullptr::operator==(const Nullptr &rhs) const { return true; }

bool Nullptr::operator!=(const Nullptr &rhs) const { return !(*this == rhs); }