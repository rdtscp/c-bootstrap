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

  bool operator==(const StringLiteral &d) const { return true; }

  std::string astClass() const override { return "StringLiteral"; }
};

}; // namespace ACC

#endif