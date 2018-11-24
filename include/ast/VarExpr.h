#pragma once

#ifndef VAREXPR_H
#define VAREXPR_H

#include <string>

#include "Expr.h"

namespace ACC {

class VarExpr : public Expr {

public:
  std::string identifier;

  VarExpr(std::string identifier) : identifier(identifier) {}

  bool operator==(const VarExpr &d) const { return true; }

  std::string astClass() const override { return "VarExpr"; }
};

}; // namespace ACC

#endif