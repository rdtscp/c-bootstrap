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

  std::string accept(ASTVisitor &v) override { return v.visit(*this); }
  std::string astClass() const override { return "Assign"; }
};

}; // namespace ACC

#endif