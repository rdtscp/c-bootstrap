#pragma once

#ifndef RETURN_H
#define RETURN_H

#include <string>

namespace ACC {

class Return : public Stmt {

public:
  std::shared_ptr<Expr> returnExpr = nullptr;

  Return() {
    // Boilerplate Code.
  }

  Return(std::shared_ptr<Expr> returnExpr) : returnExpr(returnExpr) {
    // Boilerplate Code.
  }

  bool operator==(const Return &d) const { return true; }

  virtual std::string toString() const { return "Return"; }
};

}; // namespace ACC

#endif