#pragma once




#include "atl/include/shared_ptr.h"
#include "atl/include/string.h"

namespace ACC {

class Identifier {
  class Decl;

public:
  atl::string value;
  atl::shared_ptr<Identifier> parentIdentifier;
  atl::shared_ptr<Decl> identifierDecl;

  Identifier();

  Identifier(const atl::string &p_value);

  Identifier(const atl::string &p_value,
             const atl::shared_ptr<Identifier> &p_parentIdentifier);

  atl::string toString() const;

  bool operator==(const Identifier &rhs) const;
  bool operator!=(const Identifier &rhs) const;
};

} // namespace ACC

