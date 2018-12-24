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

  void accept(ASTVisitor<void> &v) override { return v.visit(*this); }
  std::string accept(ASTVisitor<std::string> &v) override {
    return v.visit(*this);
  }
  std::shared_ptr<Type> accept(ASTVisitor<std::shared_ptr<Type>> &v) override {
    return v.visit(*this);
  }
  MIPS::Register accept(ASTVisitor<MIPS::Register> &v) override {
    return v.visit(*this);
  }
  X86::Register accept(ASTVisitor<X86::Register> &v) override {
    return v.visit(*this);
  }
  std::string astClass() const override { return "ParenthExpr"; }
};

}; // namespace ACC

#endif