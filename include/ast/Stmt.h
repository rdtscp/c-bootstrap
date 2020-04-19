#pragma once

#include "ASTNode.h"

namespace ACC {

class Stmt : public ASTNode {
public:
  virtual bool operator==(const Stmt &rhs) const = 0;
  virtual bool operator!=(const Stmt &rhs) const = 0;
};

} // namespace ACC
