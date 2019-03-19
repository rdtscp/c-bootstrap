#include "../../include/ast/Identifier.h"

using namespace ACC;

Identifier::Identifier() : identifier(), parentIdentifier(nullptr) {}

Identifier::Identifier(const atl::string &identifier)
    : identifier(identifier), parentIdentifier(nullptr) {}

Identifier::Identifier(const atl::string &identifier,
                       const atl::shared_ptr<Identifier> &parentIdentifier)
    : identifier(identifier), parentIdentifier(parentIdentifier) {}

atl::string Identifier::toString() const { return identifier; }

bool Identifier::operator==(const Identifier &rhs) const {
  if (identifier != rhs.identifier)
    return false;

  return true;
}

bool Identifier::operator!=(const Identifier &rhs) const {
  return !(*this == rhs);
}
