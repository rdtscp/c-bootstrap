#pragma once

#ifndef ACC_TYPE_H
#define ACC_TYPE_H

#include "../ASTNode.h"
#include "atl/include/set.h"

namespace ACC {

class Type : public ASTNode {
public:
  enum class Modifiers { CONST };
  atl::set<Modifiers> typeModifiers;
  virtual ~Type() {}
  virtual int getBytes() const = 0;
  virtual bool operator==(Type &rhs) const = 0;
  virtual bool operator!=(Type &rhs) const = 0;
};

}; // namespace ACC

#endif