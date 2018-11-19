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

  bool operator==(const Expr &d) const { return true; }

  virtual std::string toString() const { return "Expr"; }
};

}; // namespace ACC

#endif