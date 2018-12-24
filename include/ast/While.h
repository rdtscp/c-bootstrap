#pragma once

#ifndef WHILE_H
#define WHILE_H

#include <string>

namespace ACC {

class While : public Stmt {

public:
  std::shared_ptr<Stmt> body;
  std::shared_ptr<Expr> condition;

  While(std::shared_ptr<Stmt> body, std::shared_ptr<Expr> condition)
      : body(body), condition(condition) {}

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
  std::string astClass() const override { return "While"; }
};

}; // namespace ACC

#endif