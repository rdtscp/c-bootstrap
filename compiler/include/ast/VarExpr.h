#pragma once

#ifndef ACC_VAREXPR_H
#define ACC_VAREXPR_H

#include "Expr.h"

namespace ACC {

class VarExpr : public Expr, public std::enable_shared_from_this<VarExpr> {

public:
  atl::string identifier;
  std::shared_ptr<VarDecl> variableDecl;

  VarExpr(const atl::string &identifier);

  std::shared_ptr<VarExpr> getptr();

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