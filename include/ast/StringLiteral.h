#pragma once

#ifndef STRINGLITERAL_H
#define STRINGLITERAL_H

#include <string>

#include "Expr.h"

namespace ACC {

class StringLiteral : public Expr {

public:
  std::string value;

  StringLiteral(std::string literal) : value(literal) {}

  std::string accept(ASTVisitor &v) override { return v.visit(*this); }
  std::string astClass() const override { return "StringLiteral"; }
};

}; // namespace ACC

#endif