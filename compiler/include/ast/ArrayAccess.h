#pragma once

#ifndef ACC_ARRAYACCESS_H
#define ACC_ARRAYACCESS_H

#include "atl/include/string.h"

#include "Expr.h"

namespace ACC {

class ArrayAccess : public Expr,
                    public atl::enable_shared_from_this<ArrayAccess> {

public:
  atl::shared_ptr<Expr> array;
  atl::shared_ptr<Expr> index;

  ArrayAccess(const atl::shared_ptr<Expr> &p_array,
              const atl::shared_ptr<Expr> &p_index);

  atl::shared_ptr<ArrayAccess> getptr();

  bool operator==(Expr &rhs) const override;
  bool operator!=(Expr &rhs) const override;

  bool operator==(const ArrayAccess &rhs) const;
  bool operator!=(const ArrayAccess &rhs) const;

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