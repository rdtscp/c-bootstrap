#pragma once

#ifndef PARENTHEXPR_H
#define PARENTHEXPR_H

#include <string>

#include "Expr.h"

namespace ACC {

class ParenthExpr : public Expr {

public:
  std::shared_ptr<Expr> innerExpr;

  ParenthExpr(std::shared_ptr<Expr> innerExpr) : innerExpr(innerExpr) {}

  void accept(ASTVisitor &v) override { v.visit(*this); }
  std::string astClass() const override { return "ParenthExpr"; }
};

}; // namespace ACC

#endif