#include "ast/EnumClassTypeDecl.h"

using namespace ACC;

EnumClassTypeDecl::EnumClassTypeDecl(
    const atl::shared_ptr<Identifier> &p_identifier,
    const atl::unordered_map<atl::string, atl::string> &p_states)
    : identifier(p_identifier), states(p_states) {}

// unsigned int EnumClassTypeDecl::getBytes() const { return 4u; }

atl::shared_ptr<Identifier> EnumClassTypeDecl::getIdentifier() const {
  return identifier;
}

bool EnumClassTypeDecl::operator==(Decl &rhs) const {
  if (rhs.astClass() == astClass())
    return *this == *static_cast<EnumClassTypeDecl *>(&rhs);
  return false;
}

bool EnumClassTypeDecl::operator!=(Decl &rhs) const { return !(*this == rhs); }

bool EnumClassTypeDecl::operator==(const EnumClassTypeDecl &rhs) const {
  return states == rhs.states;
}

bool EnumClassTypeDecl::operator!=(const EnumClassTypeDecl &rhs) const {
  return !(*this == rhs);
}
