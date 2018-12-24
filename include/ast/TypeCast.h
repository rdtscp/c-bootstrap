#pragma once

#ifndef TYPECAST_H
#define TYPECAST_H

#include <string>

#include "Expr.h"
#include "Type.h"

namespace ACC {

class TypeCast : public Expr {

public:
  std::shared_ptr<Type> type;
  std::shared_ptr<Expr> expr;

  TypeCast(std::shared_ptr<Type> type, std::shared_ptr<Expr> expr)
      : type(type), expr(expr) {}

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
  std::string astClass() const override { return "TypeCast"; }
};

}; // namespace ACC

#endif