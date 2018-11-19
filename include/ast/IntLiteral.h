#pragma once

#ifndef INTLITERAL_H
#define INTLITERAL_H

#include <string>

#include "Expr.h"

namespace ACC {

class IntLiteral : public Expr {

public:
  std::string value;

  IntLiteral(std::string literal) : value(literal) {
    // Boilerplate Code.
  }

  bool operator==(const IntLiteral &d) const { return true; }

  virtual std::string toString() const { return "IntLiteral(" + value + ")"; }
};

}; // namespace ACC

#endif