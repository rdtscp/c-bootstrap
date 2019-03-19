#pragma once

#ifndef ACC_IDENTIFIER_H
#define ACC_IDENTIFIER_H

#include "atl/include/shared_ptr.h"
#include "atl/include/string.h"

namespace ACC {

class Identifier {
  class Decl;

public:
  atl::string identifier;
  atl::shared_ptr<Identifier> parentIdentifier;
  atl::shared_ptr<Decl> identifierDecl;

  Identifier();

  Identifier(const atl::string &identifier);

  Identifier(const atl::string &identifier,
             const atl::shared_ptr<Identifier> &parentIdentifier);

  atl::string toString() const;

  bool operator==(const Identifier &rhs) const;
  bool operator!=(const Identifier &rhs) const;
};

} // namespace ACC

#endif