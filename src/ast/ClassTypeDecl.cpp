#include "ast/ClassTypeDecl.h"

#include "ast/ClassType.h"

using namespace ACC;

ClassTypeDecl::ClassTypeDecl(const atl::shared_ptr<ClassType> &p_classType)
    : classType(p_classType) {}

atl::shared_ptr<Identifier> ClassTypeDecl::getIdentifier() const {
  return classType->identifier;
}

bool ClassTypeDecl::operator==(Decl &rhs) const {
  if (rhs.astClass() == astClass())
    return *this == *static_cast<ClassTypeDecl *>(&rhs);
  return false;
}

bool ClassTypeDecl::operator!=(Decl &rhs) const { return !(*this == rhs); }

bool ClassTypeDecl::operator==(const ClassTypeDecl &rhs) const {
  if (*classType != *rhs.classType)
    return false;

  return true;
}

bool ClassTypeDecl::operator!=(const ClassTypeDecl &rhs) const {
  return !(*this == rhs);
}
