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

  std::string accept(ASTVisitor &v) override { return v.visit(*this); }
  std::string astClass() const override { return "Return"; }
};

}; // namespace ACC

#endif