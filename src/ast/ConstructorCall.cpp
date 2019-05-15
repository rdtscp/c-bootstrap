#include "ast/ConstructorCall.h"

using namespace ACC;

ConstructorCall::ConstructorCall(
    const atl::shared_ptr<Identifier> &p_ctorIdentifier,
    const atl::vector<atl::shared_ptr<Expr>> &p_ctorArgs)
    : constructorIdentifier(p_ctorIdentifier), constructorArgs(p_ctorArgs) {}

bool ConstructorCall::operator==(Expr &rhs) const {
  if (rhs.astClass() == astClass())
    return *this == *static_cast<ConstructorCall *>(&rhs);
  return false;
}

bool ConstructorCall::operator!=(Expr &rhs) const { return !(*this == rhs); }

bool ConstructorCall::operator==(const ConstructorCall &rhs) const {
  if (*constructorIdentifier != *rhs.constructorIdentifier)
    return false;

  if (constructorArgs.size() != rhs.constructorArgs.size())
    return false;

  for (unsigned int i = 0; i < constructorArgs.size(); ++i) {
    if (*constructorArgs[i] != *rhs.constructorArgs[i])
      return false;
  }

  return true;
}

bool ConstructorCall::operator!=(const ConstructorCall &rhs) const {
  return !(*this == rhs);
}
