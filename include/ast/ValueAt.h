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

  bool operator==(const ValueAt &d) const { return true; }

  std::string astClass() const override { return "ValueAt"; }
};

}; // namespace ACC

#endif