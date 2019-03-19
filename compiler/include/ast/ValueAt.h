#pragma once

#ifndef ACC_VALUEAT_H
#define ACC_VALUEAT_H

#include "Expr.h"

namespace ACC {

class ValueAt : public Expr, public atl::enable_shared_from_this<ValueAt> {

public:
  atl::shared_ptr<Expr> derefExpr;

  ValueAt(const atl::shared_ptr<Expr> &derefExpr);

  atl::shared_ptr<ValueAt> getptr();

  bool operator==(Expr &rhs) const override;
  bool operator!=(Expr &rhs) const override;

  bool operator==(const ValueAt &rhs) const;
  bool operator!=(const ValueAt &rhs) const;

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