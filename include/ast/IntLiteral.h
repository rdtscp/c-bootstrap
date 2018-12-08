#pragma once

#ifndef INTLITERAL_H
#define INTLITERAL_H

#include <string>

#include "Expr.h"

namespace ACC {

class IntLiteral : public Expr {

public:
  std::string value;

  IntLiteral(std::string literal) : value(literal) {}

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
  std::string astClass() const override { return "IntLiteral"; }
};

}; // namespace ACC

#endif