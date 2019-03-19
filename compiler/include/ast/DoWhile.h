#pragma once

#ifndef ACC_DOWHILE_H
#define ACC_DOWHILE_H

#include "Expr.h"
#include "Stmt.h"

namespace ACC {

class DoWhile : public Stmt, public atl::enable_shared_from_this<DoWhile> {

public:
  atl::shared_ptr<Stmt> body;
  atl::shared_ptr<Expr> condition;

  DoWhile(const atl::shared_ptr<Stmt> &p_body,
          const atl::shared_ptr<Expr> &p_condition);

  atl::shared_ptr<DoWhile> getptr();

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