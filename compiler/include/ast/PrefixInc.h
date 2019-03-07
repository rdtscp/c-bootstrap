#pragma once

#ifndef ACC_PREFIXINC_H
#define ACC_PREFIXINC_H

#include "Expr.h"
#include "VarExpr.h"

namespace ACC {

class PrefixInc : public Expr, public atl::enable_shared_from_this<PrefixInc> {

public:
  atl::shared_ptr<VarExpr> incrementVar;

  PrefixInc(const atl::shared_ptr<VarExpr> &incrementVar);
  virtual ~PrefixInc() {}

  atl::shared_ptr<PrefixInc> getptr();

  bool operator==(Expr &rhs) const override;
  bool operator!=(Expr &rhs) const override;

  bool operator==(const PrefixInc &rhs) const;
  bool operator!=(const PrefixInc &rhs) const;

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