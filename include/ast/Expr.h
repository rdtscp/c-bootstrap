#pragma once

#include "Stmt.h"

namespace ACC {

class Expr : public Stmt {
public:
  atl::shared_ptr<Type> exprType;

  virtual ~Expr() {}
  virtual bool operator==(Expr &rhs) const = 0;
  virtual bool operator!=(Expr &rhs) const = 0;
};

} // namespace ACC
