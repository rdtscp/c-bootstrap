#pragma once

#ifndef RETURN_H
#define RETURN_H

#include <string>

namespace ACC {

class Return : public Stmt {

public:
  std::shared_ptr<Expr> returnExpr = nullptr;

  Return() {}
  Return(std::shared_ptr<Expr> returnExpr) : returnExpr(returnExpr) {}

  void accept(ASTVisitor<void> &v) override { return v.visit(*this); }
  std::string accept(ASTVisitor<std::string> &v) override {
    return v.visit(*this);
  }
  std::shared_ptr<Type> accept(ASTVisitor<std::shared_ptr<Type>> &v) override {
    return v.visit(*this);
  }
  std::string astClass() const override { return "Return"; }
};

}; // namespace ACC

#endif