#pragma once

#ifndef ACC_DOWHILE_H
#define ACC_DOWHILE_H

#include <string>

#include "Expr.h"
#include "Stmt.h"

namespace ACC {

class DoWhile : public Stmt, public std::enable_shared_from_this<DoWhile> {

public:
  std::shared_ptr<Stmt> body;
  std::shared_ptr<Expr> condition;

  DoWhile(std::shared_ptr<Stmt> body, std::shared_ptr<Expr> condition);

  std::shared_ptr<DoWhile> getptr();

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