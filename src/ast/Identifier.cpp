#include "ast/Identifier.h"

using namespace ACC;

Identifier::Identifier(const atl::string &p_value)
    : value(p_value), tailIdentifier(nullptr) {}

atl::shared_ptr<Identifier> Identifier::clone() const {
  atl::shared_ptr<Identifier> output(new Identifier(value));
  atl::shared_ptr<Identifier> tail = tailIdentifier;
  while (tail) {
    output->insert(atl::shared_ptr<Identifier>(new Identifier(tail->value)));
    tail = tail->tailIdentifier;
  }
  return output;
}

void Identifier::insert(const atl::shared_ptr<Identifier> &p_tailIdentifier) {
  if (tailIdentifier) {
    tailIdentifier->insert(p_tailIdentifier);
  } else {
    tailIdentifier = p_tailIdentifier;
  }
}

const atl::string Identifier::head() const {
  return value;
}

const atl::shared_ptr<Identifier> Identifier::tail() const {
  return tailIdentifier;
}

const unsigned int Identifier::size() const {
  unsigned int output = 1;
  atl::shared_ptr<Identifier> currTail = tailIdentifier;
  while (currTail != nullptr) {
    ++output;
    currTail = currTail->tailIdentifier;
  }
  return output;
}

atl::string Identifier::toString() const {
  atl::string output = value;
  if (tailIdentifier) {
    output += "::" + tailIdentifier->toString();
  }
  return output;
}

bool Identifier::operator==(const Identifier &rhs) const {
  if (value != rhs.value)
    return false;

  if (tailIdentifier && rhs.tailIdentifier) {
    if (*tailIdentifier != *rhs.tailIdentifier) {
      return false;
    }
  }
  else if (tailIdentifier && !rhs.tailIdentifier) {
    return false;
  }
  else if (!tailIdentifier && rhs.tailIdentifier) {
    return false;
  }

  return true;
}

bool Identifier::operator!=(const Identifier &rhs) const {
  return !(*this == rhs);
}
