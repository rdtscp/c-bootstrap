#include "ast/DestructorDecl.h"

using namespace ACC;

DestructorDecl::DestructorDecl(const atl::shared_ptr<Type> &p_classType)
    : classType(p_classType) {}

atl::shared_ptr<Identifier> DestructorDecl::getIdentifier() const {
  return classType->identifier;
}


bool DestructorDecl::operator==(Decl &rhs) const {
  if (rhs.astClass() == astClass())
    return *this == *static_cast<FunDecl *>(&rhs);
  return false;
}

bool DestructorDecl::operator!=(Decl &rhs) const { return !(*this == rhs); }

bool DestructorDecl::operator==(const DestructorDecl &rhs) const {
  if (*classType != *rhs.classType)
    return false;

  return true;
}

bool DestructorDecl::operator!=(const DestructorDecl &rhs) const {
  return !(*this == rhs);
}

