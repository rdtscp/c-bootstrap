#pragma once

#include "ASTNode.h"
#include "ast/Identifier.h"

namespace ACC {

class Type : public ASTNode {
public:
  enum class Modifiers { CONST };
  atl::set<Modifiers> typeModifiers;
  virtual ~Type() {}
  virtual int getBytes() const = 0;
  virtual atl::string getSignature() const = 0;
  virtual bool operator==(Type &rhs) const = 0;
  virtual bool operator!=(Type &rhs) const = 0;
};

} // namespace ACC