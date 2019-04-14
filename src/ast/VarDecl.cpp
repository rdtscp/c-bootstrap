#include "ast/VarDecl.h"

using namespace ACC;

VarDecl::VarDecl(const atl::shared_ptr<Type> &p_type,
                 const atl::shared_ptr<Identifier> &p_identifier)
    : type(p_type), identifier(p_identifier) {}

int VarDecl::getBytes() const { return type->getBytes(); }

atl::shared_ptr<Identifier> VarDecl::getIdentifier() const {
  return identifier;
}

bool VarDecl::operator==(Decl &rhs) const {
  if (rhs.astClass() == astClass())
    return *this == *static_cast<VarDecl *>(&rhs);
  return false;
}

bool VarDecl::operator!=(Decl &rhs) const { return !(*this == rhs); }

bool VarDecl::operator==(const VarDecl &rhs) const {
  if (*type != *rhs.type)
    return false;

  if (*identifier != *rhs.identifier)
    return false;

  return true;
}

bool VarDecl::operator!=(const VarDecl &rhs) const { return !(*this == rhs); }
