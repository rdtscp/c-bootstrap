#pragma once

#ifndef EXPR_H
#define EXPR_H

#include <string>

#include "Stmt.h"

namespace ACC {

class Expr : public Stmt {

public:
  Expr() {
    // Boilerplate Code.
  }

  bool operator==(const Expr &d) const { return false; }

  std::string toString() const override { return "Expr"; }

  std::string strVal() const override { return "Expr"; }
};

}; // namespace ACC

#endif