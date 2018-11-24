#pragma once

#ifndef FUNCALL_H
#define FUNCALL_H

#include <string>
#include <vector>

#include "Expr.h"

namespace ACC {

class FunCall : public Expr {

public:
  std::string funName;
  std::vector<std::shared_ptr<Expr>> funArgs;

  FunCall(std::string funName, std::vector<std::shared_ptr<Expr>> funArgs)
      : funName(funName), funArgs(funArgs) {}

  void accept(ASTVisitor &v) override { v.visit(*this); }
  std::string astClass() const override { return "FunCall"; }
};

}; // namespace ACC

#endif