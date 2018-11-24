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

  std::string accept(ASTVisitor &v) override { return v.visit(*this); }
  std::string astClass() const override { return "TypeCast"; }
};

}; // namespace ACC

#endif