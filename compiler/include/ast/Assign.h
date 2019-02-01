#pragma once

#ifndef ACC_ASSIGN_H
#define ACC_ASSIGN_H

#include "Expr.h"
#include "Stmt.h"

namespace ACC {

class Assign : public Stmt, public std::enable_shared_from_this<Assign> {

public:
  std::shared_ptr<Expr> lhs;
  std::shared_ptr<Expr> rhs;

  Assign(std::shared_ptr<Expr> lhs, std::shared_ptr<Expr> rhs);

  std::shared_ptr<Assign> getptr();

  void accept(ASTVisitor<void> &v) override;
  atl::string accept(ASTVisitor<atl::string> &v) override;
  std::shared_ptr<Type> accept(ASTVisitor<std::shared_ptr<Type>> &v) override;
  MIPS::Register accept(ASTVisitor<MIPS::Register> &v) override;
  std::shared_ptr<X86::Operand>
  accept(ASTVisitor<std::shared_ptr<X86::Operand>> &v) override;
  std::shared_ptr<ASTNode>
  accept(ASTVisitor<std::shared_ptr<ASTNode>> &v) override;
  atl::string astClass() const override;
};

}; // namespace ACC

#endif