#pragma once

#ifndef BINOP_H
#define BINOP_H

#include <string>

#include "Expr.h"
#include "Op.h"

namespace ACC {

class BinOp : public Expr {

public:
  std::shared_ptr<Expr> lhs;
  Op operation;
  std::shared_ptr<Expr> rhs;

  BinOp(std::shared_ptr<Expr> lhs, Op operation, std::shared_ptr<Expr> rhs)
      : lhs(lhs), operation(operation), rhs(rhs) {
    // Boilerplate Code.
  }

  bool operator==(const BinOp &d) const { return true; }

  virtual std::string toString() const { return "BinOp"; }
};

}; // namespace ACC

#endif