#pragma once

#ifndef ASSIGN_H
#define ASSIGN_H

#include <string>

#include "Expr.h"
#include "Stmt.h"

namespace ACC {

class Assign : public Stmt {

public:
  std::shared_ptr<Expr> lhs;
  std::shared_ptr<Expr> rhs;

  Assign(std::shared_ptr<Expr> lhs, std::shared_ptr<Expr> rhs);

  void accept(ASTVisitor<void> &v) override;
  std::string accept(ASTVisitor<std::string> &v) override;
  std::shared_ptr<Type> accept(ASTVisitor<std::shared_ptr<Type>> &v) override;
  MIPS::Register accept(ASTVisitor<MIPS::Register> &v) override;
  std::shared_ptr<X86::Operand> accept(ASTVisitor<std::shared_ptr<X86::Operand>> &v) override;
  std::string astClass() const override;
};

}; // namespace ACC

#endif