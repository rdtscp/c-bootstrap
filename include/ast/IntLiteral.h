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

  void accept(ASTVisitor &v) override { v.visit(*this); }
  std::string astClass() const override { return "IntLiteral"; }
};

}; // namespace ACC

#endif