#pragma once

#ifndef ACC_VAREXPR_H
#define ACC_VAREXPR_H

#include <string>

#include "Expr.h"

namespace ACC {

class VarExpr : public Expr, public std::enable_shared_from_this<VarExpr> {

public:
  std::string identifier;
  std::shared_ptr<VarDecl> variableDecl;

  VarExpr(std::string identifier);

  std::shared_ptr<VarExpr> getptr();

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