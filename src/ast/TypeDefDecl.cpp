#include "ast/TypeDefDecl.h"

using namespace ACC;

TypeDefDecl::TypeDefDecl(const atl::shared_ptr<Type> &p_type,
                         const atl::shared_ptr<Identifier> &p_identifier)
    : type(p_type), identifier(p_identifier) {}

atl::shared_ptr<Identifier> TypeDefDecl::getIdentifier() const {
  return identifier;
}

bool TypeDefDecl::operator==(Decl &rhs) const {
  if (rhs.astClass() == astClass())
    return *this == *static_cast<TypeDefDecl *>(&rhs);
  return false;
}

bool TypeDefDecl::operator!=(Decl &rhs) const { return !(*this == rhs); }

bool TypeDefDecl::operator==(const TypeDefDecl &rhs) const {
  if (*type != *rhs.type)
    return false;

  if (*identifier != *rhs.identifier)
    return false;

  return true;
}

bool TypeDefDecl::operator!=(const TypeDefDecl &rhs) const {
  return !(*this == rhs);
}
