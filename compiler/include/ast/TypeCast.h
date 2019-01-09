#pragma once

#ifndef ACC_TYPECAST_H
#define ACC_TYPECAST_H

#include <string>

#include "Expr.h"
#include "Type.h"

namespace ACC {

class TypeCast : public Expr, public std::enable_shared_from_this<TypeCast> {

public:
  std::shared_ptr<Type> type;
  std::shared_ptr<Expr> expr;

  TypeCast(std::shared_ptr<Type> type, std::shared_ptr<Expr> expr);

  std::shared_ptr<TypeCast> getptr();

  void accept(ASTVisitor<void> &v) override;
  std::string accept(ASTVisitor<std::string> &v) override;
  std::shared_ptr<Type> accept(ASTVisitor<std::shared_ptr<Type>> &v) override;
  MIPS::Register accept(ASTVisitor<MIPS::Register> &v) override;
  std::shared_ptr<X86::Operand>
  accept(ASTVisitor<std::shared_ptr<X86::Operand>> &v) override;
  std::shared_ptr<ASTNode>
  accept(ASTVisitor<std::shared_ptr<ASTNode>> &v) override;
  std::string astClass() const override;
};

}; // namespace ACC

#endif