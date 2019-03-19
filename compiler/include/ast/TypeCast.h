#pragma once

#ifndef ACC_TYPECAST_H
#define ACC_TYPECAST_H

#include "Expr.h"
#include "Type.h"

namespace ACC {

class TypeCast : public Expr, public atl::enable_shared_from_this<TypeCast> {

public:
  atl::shared_ptr<Type> type;
  atl::shared_ptr<Expr> expr;

  TypeCast(const atl::shared_ptr<Type> &p_type,
           const atl::shared_ptr<Expr> &p_expr);

  atl::shared_ptr<TypeCast> getptr();

  bool operator==(Expr &rhs) const override;
  bool operator!=(Expr &rhs) const override;

  bool operator==(const TypeCast &rhs) const;
  bool operator!=(const TypeCast &rhs) const;

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