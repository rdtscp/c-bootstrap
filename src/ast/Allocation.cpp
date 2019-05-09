#include "ast/Allocation.h"

using namespace ACC;

Allocation::Allocation(const atl::shared_ptr<Type> &p_varType)
    : varType(p_varType), varConstructorCall(nullptr) {}

Allocation::Allocation(const atl::shared_ptr<FunCall> &p_varConstructorCall)
    : varType(nullptr), varConstructorCall(p_varConstructorCall) {}

atl::string Allocation::getSignature() const {
  if (varType != nullptr)
    return varType->getSignature() + "*";

  return varConstructorCall->getSignature() + "*";
}

bool Allocation::operator==(Expr &rhs) const {
  if (rhs.astClass() == astClass())
    return *this == *static_cast<Allocation *>(&rhs);
  return false;
}

bool Allocation::operator!=(Expr &rhs) const { return !(*this == rhs); }

bool Allocation::operator==(const Allocation &rhs) const {
  if (*varType != *rhs.varType)
    return false;

  if (varConstructorCall != rhs.varConstructorCall)
    return false;

  return true;
}

bool Allocation::operator!=(const Allocation &rhs) const {
  return !(*this == rhs);
}
