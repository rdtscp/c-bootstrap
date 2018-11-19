#pragma once

#ifndef VAREXPR_H
#define VAREXPR_H

#include <string>

#include "Expr.h"

namespace ACC {

class VarExpr : public Expr {

public:
  std::string identifier;

  VarExpr(std::string identifier) : identifier(identifier) {
    // Boilerplate Code.
  }

  bool operator==(const VarExpr &d) const { return true; }

  virtual std::string toString() const { return "VarExpr(" + identifier + ")"; }
};

}; // namespace ACC

#endif