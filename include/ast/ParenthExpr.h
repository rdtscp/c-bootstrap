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

  virtual std::string toString() const { return "ParenthExpr"; }
};

}; // namespace ACC

#endif