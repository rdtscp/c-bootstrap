#pragma once

#ifndef ACC_WHILE_H
#define ACC_WHILE_H

#include <string>

#include "Expr.h"
#include "Stmt.h"

namespace ACC {

class While : public Stmt, public std::enable_shared_from_this<While> {

public:
  std::shared_ptr<Stmt> body;
  std::shared_ptr<Expr> condition;

  While(std::shared_ptr<Stmt> body, std::shared_ptr<Expr> condition);

  std::shared_ptr<While> getptr();

  void accept(ASTVisitor<void> &v) override;
  std::string accept(ASTVisitor<std::string> &v) override;
  std::shared_ptr<Type> accept(ASTVisitor<std::shared_ptr<Type>> &v) override;
  MIPS::Register accept(ASTVisitor<MIPS::Register> &v) override;
  std::shared_ptr<X86::Operand>
  accept(ASTVisitor<std::shared_ptr<X86::Operand>> &v) override;
  std::shared_ptr<ASTNode>
  accept(ASTVisitor<std::shared_ptr<ASTNode>> &v) override;
  std::string astClass() const override;
};

}; // namespace ACC

#endif