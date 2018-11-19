#pragma once

#ifndef CHARLITERAL_H
#define CHARLITERAL_H

#include <string>

#include "Expr.h"

namespace ACC {

class CharLiteral : public Expr {

public:
  std::string value;

  CharLiteral(std::string literal) : value(literal) {
    // Boilerplate Code.
  }

  bool operator==(const CharLiteral &d) const { return true; }

  virtual std::string toString() const { return "CharLiteral(" + value + ")"; }
};

}; // namespace ACC

#endif