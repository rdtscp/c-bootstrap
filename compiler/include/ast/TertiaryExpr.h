#pragma once

#ifndef ACC_TERTIARY_H
#define ACC_TERTIARY_H

#include "Expr.h"

namespace ACC {

class TertiaryExpr : public Expr,
                     public atl::enable_shared_from_this<TertiaryExpr> {

public:
  atl::shared_ptr<Expr> tertiaryCondition;
  atl::shared_ptr<Expr> tertiaryIfBody;
  atl::shared_ptr<Expr> tertiaryElseBody;

  TertiaryExpr(const atl::shared_ptr<Expr> &p_tertiaryCondition,
               const atl::shared_ptr<Expr> &p_tertiaryIfBody,
               const atl::shared_ptr<Expr> &p_tertiaryElseBody);

  atl::shared_ptr<TertiaryExpr> getptr();

  bool operator==(Expr &rhs) const override;
  bool operator!=(Expr &rhs) const override;

  bool operator==(const TertiaryExpr &rhs) const;
  bool operator!=(const TertiaryExpr &rhs) const;

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