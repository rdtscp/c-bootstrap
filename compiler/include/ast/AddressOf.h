#pragma once

#ifndef ACC_ADDRESSOF_H
#define ACC_ADDRESSOF_H

#include "Expr.h"

namespace ACC {

class AddressOf : public Expr, public atl::enable_shared_from_this<AddressOf> {

public:
  atl::shared_ptr<Expr> addressOfExpr;

  AddressOf(const atl::shared_ptr<Expr> &p_addressOfExpr);

  atl::shared_ptr<AddressOf> getptr();

  bool operator==(Expr &rhs) const override;
  bool operator!=(Expr &rhs) const override;

  bool operator==(const AddressOf &rhs) const;
  bool operator!=(const AddressOf &rhs) const;

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