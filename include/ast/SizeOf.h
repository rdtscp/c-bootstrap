#pragma once

#ifndef SIZEOF_H
#define SIZEOF_H

#include <string>

#include "Expr.h"
#include "Type.h"

namespace ACC {

class SizeOf : public Expr {

public:
  std::shared_ptr<Type> type;

  SizeOf(std::shared_ptr<Type> type) : type(type) {}

  void accept(ASTVisitor<void> &v) override { return v.visit(*this); }
  std::string accept(ASTVisitor<std::string> &v) override {
    return v.visit(*this);
  }
  std::shared_ptr<Type> accept(ASTVisitor<std::shared_ptr<Type>> &v) override {
    return v.visit(*this);
  }
  std::string astClass() const override { return "SizeOf"; }
};

}; // namespace ACC

#endif