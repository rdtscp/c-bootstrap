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

  std::string accept(ASTVisitor &v) override { return v.visit(*this); }
  std::string astClass() const override { return "VarExpr"; }
};

}; // namespace ACC

#endif