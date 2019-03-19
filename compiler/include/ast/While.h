#pragma once

#ifndef ACC_WHILE_H
#define ACC_WHILE_H

#include "Expr.h"
#include "Stmt.h"

namespace ACC {

class While : public Stmt, public atl::enable_shared_from_this<While> {

public:
  atl::shared_ptr<Stmt> body;
  atl::shared_ptr<Expr> condition;

  While(const atl::shared_ptr<Stmt> &p_body,
        const atl::shared_ptr<Expr> &p_condition);

  atl::shared_ptr<While> getptr();

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

} // namespace ACC

#endif