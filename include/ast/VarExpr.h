#pragma once

#ifndef VAREXPR_H
#define VAREXPR_H

#include <string>

#include "Expr.h"

namespace ACC {

class VarExpr : public Expr {

public:
  std::string identifier;
  std::shared_ptr<VarDecl> variableDecl;

  VarExpr(std::string identifier) : identifier(identifier) {}

  void accept(ASTVisitor<void> &v) override { return v.visit(*this); }
  std::string accept(ASTVisitor<std::string> &v) override {
    return v.visit(*this);
  }
  std::shared_ptr<Type> accept(ASTVisitor<std::shared_ptr<Type>> &v) override {
    return v.visit(*this);
  }
  std::string astClass() const override { return "VarExpr"; }
};

}; // namespace ACC

#endif