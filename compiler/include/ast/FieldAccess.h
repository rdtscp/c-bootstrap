#pragma once

#ifndef ACC_FIELDACCESS_H
#define ACC_FIELDACCESS_H

#include "Expr.h"

namespace ACC {

class FieldAccess : public Expr,
                    public std::enable_shared_from_this<FieldAccess> {

public:
  std::shared_ptr<Expr> object;
  atl::string field;

  FieldAccess(std::shared_ptr<Expr> object, const atl::string &field);

  std::shared_ptr<FieldAccess> getptr();

  void accept(ASTVisitor<void> &v) override;
  atl::string accept(ASTVisitor<atl::string> &v) override;
  std::shared_ptr<Type> accept(ASTVisitor<std::shared_ptr<Type>> &v) override;
  MIPS::Register accept(ASTVisitor<MIPS::Register> &v) override;
  std::shared_ptr<X86::Operand>
  accept(ASTVisitor<std::shared_ptr<X86::Operand>> &v) override;
  std::shared_ptr<ASTNode>
  accept(ASTVisitor<std::shared_ptr<ASTNode>> &v) override;
  atl::string astClass() const override;
};

}; // namespace ACC

#endif