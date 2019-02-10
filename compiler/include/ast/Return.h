#pragma once

#ifndef ACC_RETURN_H
#define ACC_RETURN_H

#include "Expr.h"
#include "Stmt.h"

namespace ACC {

class Return : public Stmt, public atl::enable_shared_from_this<Return> {

public:
  atl::shared_ptr<Expr> returnExpr;

  Return();
  Return(atl::shared_ptr<Expr> returnExpr);

  atl::shared_ptr<Return> getptr();

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