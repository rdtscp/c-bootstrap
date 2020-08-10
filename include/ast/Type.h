#pragma once

#include "ASTNode.h"
#include "ast/Identifier.h"

namespace ACC {

class Type : public ASTNode {
public:
  enum class Modifiers { CONST };
  atl::set<Modifiers> typeModifiers;
  virtual ~Type() {}
  virtual unsigned int getBytes() const = 0;
  virtual bool canCastTo(Type &rhs) const = 0;
  virtual bool equivalentTo(Type &rhs) const { return *this == rhs; }
  virtual atl::string mangle() const = 0;
  virtual bool operator==(Type &rhs) const = 0;
  virtual bool operator!=(Type &rhs) const = 0;
};

} // namespace ACC
