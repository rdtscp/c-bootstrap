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

  std::string toString() const override {
    return identifier;
  }

  std::string strVal() const override {
    return "VarExpr(" + identifier + ")";
  }
};

}; // namespace ACC

#endif