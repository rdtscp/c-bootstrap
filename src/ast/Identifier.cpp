#include "ast/Identifier.h"

using namespace ACC;

Identifier::Identifier() : value(""), parentIdentifier(nullptr) {}

Identifier::Identifier(const atl::string &p_value)
    : value(p_value), parentIdentifier(nullptr) {}

Identifier::Identifier(const atl::string &p_value,
                       const atl::shared_ptr<Identifier> &p_parentIdentifier)
    : value(p_value), parentIdentifier(p_parentIdentifier) {}

const unsigned int Identifier::namespaceCount() const {
  unsigned int output = 0;
  atl::shared_ptr<Identifier> currParent = parentIdentifier;
  while (currParent != nullptr) {
    ++output;
    currParent = currParent->parentIdentifier;
  }
  return output;
}

const atl::shared_ptr<Identifier> Identifier::namespaceHead() const {
  atl::shared_ptr<Identifier> parentIdent = parentIdentifier;
  while (parentIdent->parentIdentifier != nullptr) {
    parentIdent = parentIdent->parentIdentifier;
  }
  return parentIdent;
}

const atl::shared_ptr<Identifier> Identifier::namespaceTail() const {
  const unsigned int numNamespaces = namespaceCount();
  return deepCopyIdentifier(numNamespaces - 1);
}

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

const atl::shared_ptr<Identifier>
Identifier::deepCopyIdentifier(const unsigned int depth) const {
  // Create the new 'root' identifier.
  atl::shared_ptr<Identifier> output(new Identifier(value));

  // Alias the new Identifier.
  atl::shared_ptr<Identifier> currCopy = output;
  atl::shared_ptr<Identifier> thisParent = parentIdentifier;

  for (unsigned int idx = 0; idx < depth; ++idx) {
    // Create a new parent using the original Identifier.
    atl::shared_ptr<Identifier> newParent(
        new Identifier(thisParent->value));
    // Set the parent of the curr copy to be the new parent.
    currCopy->parentIdentifier = newParent;

    // Point the copy/original alias to their parents.
    currCopy = currCopy->parentIdentifier;
    thisParent = thisParent->parentIdentifier;
  }

  return output;
}