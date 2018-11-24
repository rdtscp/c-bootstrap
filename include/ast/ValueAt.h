#pragma once

#ifndef VALUEAT_H
#define VALUEAT_H

#include <string>

#include "Expr.h"

namespace ACC {

class ValueAt : public Expr {

public:
  std::shared_ptr<Expr> derefExpr;

  ValueAt(std::shared_ptr<Expr> derefExpr) : derefExpr(derefExpr) {}

  void accept(ASTVisitor &v) override { v.visit(*this); }
  std::string astClass() const override { return "ValueAt"; }
};

}; // namespace ACC

#endif