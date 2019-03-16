#pragma once

#ifndef ACC_IDENTIFIER_H
#define ACC_IDENTIFIER_H

#include "Decl.h"

namespace ACC {

class Identifier {

public:
  atl::string identifier;
  atl::shared_ptr<Identifier> parentIdentifier;
  atl::shared_ptr<Decl> identifierDecl;

  Identifier(const atl::string &identifier);
  Identifier(const atl::string &identifier,
             atl::shared_ptr<Identifier> parentIdentifier);

  bool operator==(const Identifier &rhs) const;
  bool operator!=(const Identifier &rhs) const;
};

}; // namespace ACC

#endif