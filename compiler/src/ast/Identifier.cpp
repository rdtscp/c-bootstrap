#include "../../include/ast/Identifier.h"

using namespace ACC;

Identifier::Identifier() : value(), parentIdentifier(nullptr) {}

Identifier::Identifier(const atl::string &p_value)
    : value(p_value), parentIdentifier(nullptr) {}

Identifier::Identifier(const atl::string &p_value,
                       const atl::shared_ptr<Identifier> &p_parentIdentifier)
    : value(p_value), parentIdentifier(p_parentIdentifier) {}

atl::string Identifier::toString() const {
  atl::string output = value;
  if (parentIdentifier) {
    output = parentIdentifier->toString() + "::" + output;
  }
  return output;
}

bool Identifier::operator==(const Identifier &rhs) const {
  if (value != rhs.value)
    return false;

  if (parentIdentifier && rhs.parentIdentifier)
    if (*parentIdentifier != *rhs.parentIdentifier)
      return false;

  if (parentIdentifier || rhs.parentIdentifier)
    return false;

  return true;
}

bool Identifier::operator!=(const Identifier &rhs) const {
  return !(*this == rhs);
}
