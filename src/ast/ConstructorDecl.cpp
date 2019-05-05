#include "ast/ConstructorDecl.h"

using namespace ACC;

ConstructorDecl::ConstructorDecl(
    const atl::shared_ptr<Type> &p_classType,
    const atl::vector<atl::shared_ptr<VarDecl>> &p_constructorParams)
    : classType(p_classType), constructorParams(p_constructorParams) {}

atl::shared_ptr<Identifier> ConstructorDecl::getIdentifier() const {
  return classType->identifier;
}

bool ConstructorDecl::operator==(Decl &rhs) const {
  if (rhs.astClass() == astClass())
    return *this == *static_cast<FunDecl *>(&rhs);
  return false;
}

bool ConstructorDecl::operator!=(Decl &rhs) const { return !(*this == rhs); }

bool ConstructorDecl::operator==(const ConstructorDecl &rhs) const {
  if (*classType != *rhs.classType)
    return false;

  if (constructorParams.size() != rhs.constructorParams.size())
    return false;

  for (unsigned int i = 0; i < constructorParams.size(); ++i)
    if (*constructorParams[i] != *rhs.constructorParams[i])
      return false;

  return true;
}

bool ConstructorDecl::operator!=(const ConstructorDecl &rhs) const {
  return !(*this == rhs);
}
