#include "ast/ConstructorDef.h"

using namespace ACC;

ConstructorDef::ConstructorDef(
    const atl::shared_ptr<Type> &p_classType,
    const atl::vector<atl::shared_ptr<VarDecl>> &p_constructorParams,
    const atl::vector<atl::shared_ptr<Assign>> &p_initialiserList,
    const atl::shared_ptr<Block> &p_constructorBlock)
    : ConstructorDecl(p_classType, p_constructorParams),
      initialiserList(p_initialiserList), constructorBlock(p_constructorBlock) {
}

atl::shared_ptr<Identifier> ConstructorDef::getIdentifier() const {
  return classType->identifier;
}

bool ConstructorDef::operator==(Decl &rhs) const {
  if (rhs.astClass() == astClass())
    return *this == *static_cast<ConstructorDef *>(&rhs);
  return false;
}

bool ConstructorDef::operator!=(Decl &rhs) const { return !(*this == rhs); }

bool ConstructorDef::operator==(const ConstructorDef &rhs) const {
  if (*classType != *rhs.classType)
    return false;

  if (constructorParams.size() != rhs.constructorParams.size())
    return false;

  for (unsigned int i = 0; i < constructorParams.size(); ++i)
    if (*constructorParams[i] != *rhs.constructorParams[i])
      return false;

  /* Check Block */
  if (*constructorBlock != *rhs.constructorBlock)
    return false;

  return true;
}

bool ConstructorDef::operator!=(const ConstructorDef &rhs) const {
  return !(*this == rhs);
}
