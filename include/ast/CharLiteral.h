#pragma once

#ifndef CHARLITERAL_H
#define CHARLITERAL_H

#include <string>

#include "Expr.h"

namespace ACC {

class CharLiteral : public Expr {

public:
  std::string value;

  CharLiteral(std::string literal) : value(literal) {}

  bool operator==(const CharLiteral &cl) const { return value == cl.value; }

  std::string astClass() const override { return "CharLiteral"; }
};

}; // namespace ACC

#endif