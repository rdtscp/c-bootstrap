#include "ast/EnumTypeDecl.h"

using namespace ACC;

EnumTypeDecl::EnumTypeDecl(const atl::shared_ptr<Identifier> &p_identifier,
                           const std::map<std::string, std::string> &p_states)
    : identifier(p_identifier), states(p_states) {}

int EnumTypeDecl::getBytes() const { return 4; }

atl::shared_ptr<Identifier> EnumTypeDecl::getIdentifier() const {
  return identifier;
}

bool EnumTypeDecl::operator==(Decl &rhs) const {
  if (rhs.astClass() == astClass())
    return *this == *static_cast<EnumTypeDecl *>(&rhs);
  return false;
}

bool EnumTypeDecl::operator!=(Decl &rhs) const { return !(*this == rhs); }

bool EnumTypeDecl::operator==(Type &rhs) const {
  if (rhs.astClass() == astClass())
    return *this == *static_cast<EnumTypeDecl *>(&rhs);
  return false;
}

bool EnumTypeDecl::operator!=(Type &rhs) const { return !(*this == rhs); }

bool EnumTypeDecl::operator==(const EnumTypeDecl &rhs) const {
  return states == rhs.states;
}

bool EnumTypeDecl::operator!=(const EnumTypeDecl &rhs) const {
  return !(*this == rhs);
}
