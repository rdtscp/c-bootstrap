#pragma once

#ifndef WHILE_H
#define WHILE_H

#include <string>

namespace ACC {

class While : public Stmt {

public:
  std::shared_ptr<Stmt> body;
  std::shared_ptr<Expr> condition;

  While(std::shared_ptr<Stmt> body, std::shared_ptr<Expr> condition)
      : body(body), condition(condition) {}

  bool operator==(const While &d) const { return true; }

  std::string astClass() const override { return "While"; }
};

}; // namespace ACC

#endif