#pragma once

#ifndef ACC_ALLOCATION_H
#define ACC_ALLOCATION_H

#include "Expr.h"

namespace ACC {

class Allocation : public Expr,
                   public atl::enable_shared_from_this<Allocation> {

public:
  atl::shared_ptr<Type> variableType;
  atl::shared_ptr<FunCall> variableConstructorCall;

  Allocation(const atl::shared_ptr<Type> &variableType);
  Allocation(const atl::shared_ptr<FunCall> &variableConstructorCall);

  atl::shared_ptr<Allocation> getptr();

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