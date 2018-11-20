#pragma once

#ifndef ASSIGN_H
#define ASSIGN_H

#include <string>

#include "Expr.h"
#include "Stmt.h"

namespace ACC {

class Assign : public Stmt {

public:
  std::shared_ptr<Expr> lhs;
  std::shared_ptr<Expr> rhs;

  Assign(std::shared_ptr<Expr> lhs, std::shared_ptr<Expr> rhs)
      : lhs(lhs), rhs(rhs) {
    // Boilerplate Code.
  }

  bool operator==(const Assign &d) const { return true; }

  std::string toString() const override {
    return lhs->toString() + " = " + rhs->toString();
  }

  std::string strVal() const override { return "Assign"; }
};

}; // namespace ACC

#endif