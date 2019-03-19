#pragma once

#ifndef ACC_SIZEOF_H
#define ACC_SIZEOF_H

#include "Expr.h"
#include "Type.h"

namespace ACC {

class SizeOf : public Expr, public atl::enable_shared_from_this<SizeOf> {

public:
  atl::shared_ptr<Type> type;

  SizeOf(const atl::shared_ptr<Type> &type);

  atl::shared_ptr<SizeOf> getptr();

  bool operator==(Expr &rhs) const override;
  bool operator!=(Expr &rhs) const override;

  bool operator==(const SizeOf &rhs) const;
  bool operator!=(const SizeOf &rhs) const;

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