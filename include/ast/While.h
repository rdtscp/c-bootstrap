#pragma once

#ifndef WHILE_H
#define WHILE_H

#include <string>

#include "Expr.h"
#include "Stmt.h"

namespace ACC {

class While : public Stmt {

public:
  std::shared_ptr<Stmt> body;
  std::shared_ptr<Expr> condition;

  While(std::shared_ptr<Stmt> body, std::shared_ptr<Expr> condition);

  void accept(ASTVisitor<void> &v) override;
  std::string accept(ASTVisitor<std::string> &v) override;
  std::shared_ptr<Type> accept(ASTVisitor<std::shared_ptr<Type>> &v) override;
  MIPS::Register accept(ASTVisitor<MIPS::Register> &v) override;
  std::shared_ptr<X86::Operand> accept(ASTVisitor<std::shared_ptr<X86::Operand>> &v) override;
  std::string astClass() const override;
};

}; // namespace ACC

#endif