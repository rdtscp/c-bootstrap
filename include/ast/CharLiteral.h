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

  bool operator==(const CharLiteral &cl) const { return value == cl.value; }

  virtual std::string toString() const override { return "'" + value + "'"; }

  std::string strVal() const override { return "CharLiteral(" + value + ")"; }

};

}; // namespace ACC

#endif