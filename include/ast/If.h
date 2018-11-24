#pragma once

#ifndef IF_H
#define IF_H

#include <string>

namespace ACC {

class If : public Stmt {

public:
  std::shared_ptr<Expr> ifCondition;
  std::shared_ptr<Stmt> ifBody;
  std::shared_ptr<Stmt> elseBody = nullptr;

  If(std::shared_ptr<Expr> ifCondition, std::shared_ptr<Stmt> ifBody)
      : ifBody(ifBody), ifCondition(ifCondition) {}

  If(std::shared_ptr<Expr> ifCondition, std::shared_ptr<Stmt> ifBody,
     std::shared_ptr<Stmt> elseBody)
      : ifBody(ifBody), ifCondition(ifCondition), elseBody(elseBody) {}

  void accept(ASTVisitor &v) override { v.visit(*this); }
  std::string astClass() const override { return "If"; }
};

}; // namespace ACC

#endif