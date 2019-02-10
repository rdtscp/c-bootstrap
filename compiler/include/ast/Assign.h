#pragma once

#ifndef ACC_ASSIGN_H
#define ACC_ASSIGN_H

#include "Expr.h"
#include "Stmt.h"

namespace ACC {

class Assign : public Stmt, public atl::enable_shared_from_this<Assign> {

public:
  atl::shared_ptr<Expr> lhs;
  atl::shared_ptr<Expr> rhs;

  Assign(atl::shared_ptr<Expr> lhs, atl::shared_ptr<Expr> rhs);

  atl::shared_ptr<Assign> getptr();

  void accept(ASTVisitor<void> &v) override;
  atl::string accept(ASTVisitor<atl::string> &v) override;
  atl::shared_ptr<Type> accept(ASTVisitor<atl::shared_ptr<Type>> &v) override;
  MIPS::Register accept(ASTVisitor<MIPS::Register> &v) override;
  atl::shared_ptr<X86::Operand>
  accept(ASTVisitor<atl::shared_ptr<X86::Operand>> &v) override;
  atl::shared_ptr<ASTNode>
  accept(ASTVisitor<atl::shared_ptr<ASTNode>> &v) override;
  atl::string astClass() const override;
};

}; // namespace ACC

#endif