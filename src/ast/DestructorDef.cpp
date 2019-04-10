#include "ast/DestructorDef.h"

using namespace ACC;

DestructorDef::DestructorDef(const atl::shared_ptr<Type> &p_classType,
                             const atl::shared_ptr<Block> &p_destructorBlock)
    : DestructorDecl(p_classType), destructorBlock(p_destructorBlock) {}

atl::shared_ptr<Identifier> DestructorDef::getIdentifier() const {
  return classType->identifier;
}

bool DestructorDef::operator==(Decl &rhs) const {
  if (rhs.astClass() == astClass())
    return *this == *static_cast<DestructorDef *>(&rhs);
  return false;
}

bool DestructorDef::operator!=(Decl &rhs) const { return !(*this == rhs); }

bool DestructorDef::operator==(const DestructorDef &rhs) const {
  if (*classType != *rhs.classType)
    return false;

  /* Check Block */
  if (*destructorBlock != *rhs.destructorBlock)
    return false;

  return true;
}

bool DestructorDef::operator!=(const DestructorDef &rhs) const {
  return !(*this == rhs);
}
