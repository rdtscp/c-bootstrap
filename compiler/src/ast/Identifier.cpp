#include "../../include/ast/Identifier.h"

using namespace ACC;

Identifier::Identifier() : value(), parentIdentifier(nullptr) {}

Identifier::Identifier(const atl::string &p_value)
    : value(p_value), parentIdentifier(nullptr) {}

Identifier::Identifier(const atl::string &p_value,
                       const atl::shared_ptr<Identifier> &p_parentIdentifier)
    : value(p_value), parentIdentifier(p_parentIdentifier) {}

atl::string Identifier::toString() const { return value; }

bool Identifier::operator==(const Identifier &rhs) const {
  if (value != rhs.value)
    return false;

  return true;
}

bool Identifier::operator!=(const Identifier &rhs) const {
  return !(*this == rhs);
}
