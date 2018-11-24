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
      : lhs(lhs), rhs(rhs) {}

  bool operator==(const Assign &d) const { return true; }

  std::string astClass() const override { return "Assign"; }
};

}; // namespace ACC

#endif