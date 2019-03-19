#pragma once

#ifndef ACC_ARRAYTYPE_H
#define ACC_ARRAYTYPE_H

#include "Expr.h"
#include "Type.h"

namespace ACC {

class ArrayType : public Type, public atl::enable_shared_from_this<ArrayType> {

public:
  atl::shared_ptr<Type> type;
  atl::shared_ptr<Expr> size;

  ArrayType(const atl::shared_ptr<Type> &type,
            const atl::shared_ptr<Expr> &size);

  atl::shared_ptr<ArrayType> getptr();

  bool operator==(Type &rhs) const override;
  bool operator!=(Type &rhs) const override;

  bool operator==(const ArrayType &rhs) const;
  bool operator!=(const ArrayType &rhs) const;

  int getBytes() const override;

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