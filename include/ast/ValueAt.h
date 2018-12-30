#pragma once

#ifndef VALUEAT_H
#define VALUEAT_H

#include <string>

#include "Expr.h"

namespace ACC {

class ValueAt : public Expr {

public:
  std::shared_ptr<Expr> derefExpr;

  ValueAt(std::shared_ptr<Expr> derefExpr);

  void accept(ASTVisitor<void> &v) override;
  std::string accept(ASTVisitor<std::string> &v) override;
  std::shared_ptr<Type> accept(ASTVisitor<std::shared_ptr<Type>> &v) override;
  MIPS::Register accept(ASTVisitor<MIPS::Register> &v) override;
  std::shared_ptr<X86::Operand> accept(ASTVisitor<std::shared_ptr<X86::Operand>> &v) override;
  std::string astClass() const override;
};

}; // namespace ACC

#endif