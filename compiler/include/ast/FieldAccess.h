#pragma once

#ifndef ACC_FIELDACCESS_H
#define ACC_FIELDACCESS_H

#include "Expr.h"

namespace ACC {

class FieldAccess : public Expr,
                    public atl::enable_shared_from_this<FieldAccess> {

public:
  atl::shared_ptr<Expr> object;
  atl::string field;

  FieldAccess(atl::shared_ptr<Expr> object, const atl::string &field);

  atl::shared_ptr<FieldAccess> getptr();

  bool operator==(Expr &rhs) const override;
  bool operator!=(Expr &rhs) const override;

  bool operator==(const FieldAccess &rhs) const;
  bool operator!=(const FieldAccess &rhs) const;

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