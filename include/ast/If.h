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
      : ifBody(ifBody), ifCondition(ifCondition) {
    // Boilerplate Code.
  }

  If(std::shared_ptr<Expr> ifCondition, std::shared_ptr<Stmt> ifBody,
     std::shared_ptr<Stmt> elseBody)
      : ifBody(ifBody), ifCondition(ifCondition), elseBody(elseBody) {
    // Boilerplate Code.
  }

  bool operator==(const If &d) const { return true; }

  std::string toString() const override {
    std::string output = "if (" + ifCondition->toString() + ")";
    output += ifBody->toString();
    if (elseBody != nullptr)
      output += " else " + elseBody->toString();
    return output;
  }

  std::string strVal() const override { return "If"; }
};

}; // namespace ACC

#endif