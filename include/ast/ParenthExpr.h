#pragma once

#ifndef PARENTHEXPR_H
#define PARENTHEXPR_H

#include <string>

#include "Expr.h"

namespace ACC {

class ParenthExpr : public Expr {

public:
  std::shared_ptr<Expr> innerExpr;

  ParenthExpr(std::shared_ptr<Expr> innerExpr) : innerExpr(innerExpr) {
    // Boilerplate Code.
  }

  bool operator==(const ParenthExpr &d) const { return true; }

  std::string toString() const override { return "(" + innerExpr->toString() + ")"; }

  std::string strVal() const override { return "ParenthExpr"; }

};

}; // namespace ACC

#endif