#pragma once

#ifndef ACC_FOR_H
#define ACC_FOR_H

#include "Expr.h"

namespace ACC {

class For : public Stmt, public atl::enable_shared_from_this<For> {

public:
  atl::shared_ptr<VarDecl> initialVarDecl;
  atl::shared_ptr<Expr> condition;
  atl::shared_ptr<Expr> endBodyExpr;
  atl::shared_ptr<Stmt> body;

  For(const atl::shared_ptr<VarDecl> &initialVarDecl,
      const atl::shared_ptr<Expr> &condition,
      const atl::shared_ptr<Expr> &endBodyExpr,
      const atl::shared_ptr<Stmt> &body);

  atl::shared_ptr<For> getptr();

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