#pragma once

#ifndef STRINGLITERAL_H
#define STRINGLITERAL_H

#include <string>

#include "Expr.h"

namespace ACC {

class StringLiteral : public Expr {

public:
  std::string value;

  StringLiteral(std::string literal) : value(literal) {
    // Boilerplate Code.
  }

  bool operator==(const StringLiteral &d) const { return true; }

  virtual std::string toString() const { return "StringLiteral(" + value + ")"; }
};

}; // namespace ACC

#endif