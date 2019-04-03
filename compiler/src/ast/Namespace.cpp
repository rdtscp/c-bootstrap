#include "ast/Namespace.h"

using namespace ACC;

Namespace::Namespace(const atl::shared_ptr<Identifier> &p_identifier,
                     const atl::vector<atl::shared_ptr<Decl>> &p_namespaceDecls)
    : identifier(p_identifier), namespaceDecls(p_namespaceDecls) {}

atl::shared_ptr<Identifier> Namespace::getIdentifier() const {
  return identifier;
}

bool Namespace::operator==(Decl &rhs) const {
  if (rhs.astClass() == astClass())
    return *this == *static_cast<Namespace *>(&rhs);
  return false;
}

bool Namespace::operator!=(Decl &rhs) const { return !(*this == rhs); }

bool Namespace::operator==(const Namespace &rhs) const {
  if (*identifier != *rhs.identifier)
    return false;

  return true;
}

bool Namespace::operator!=(const Namespace &rhs) const {
  return !(*this == rhs);
}