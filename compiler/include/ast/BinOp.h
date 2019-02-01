#pragma once

#ifndef ACC_BINOP_H
#define ACC_BINOP_H

#include "Expr.h"
#include "Op.h"

namespace ACC {

class BinOp : public Expr, public std::enable_shared_from_this<BinOp> {

public:
  std::shared_ptr<Expr> lhs;
  Op operation;
  std::shared_ptr<Expr> rhs;

  BinOp(std::shared_ptr<Expr> lhs, Op operation, std::shared_ptr<Expr> rhs);

  std::shared_ptr<BinOp> getptr();

  void accept(ASTVisitor<void> &v) override;
  atl::string accept(ASTVisitor<atl::string> &v) override;
  std::shared_ptr<Type> accept(ASTVisitor<std::shared_ptr<Type>> &v) override;
  MIPS::Register accept(ASTVisitor<MIPS::Register> &v) override;
  std::shared_ptr<X86::Operand>
  accept(ASTVisitor<std::shared_ptr<X86::Operand>> &v) override;
  std::shared_ptr<ASTNode>
  accept(ASTVisitor<std::shared_ptr<ASTNode>> &v) override;
  atl::string astClass() const override;

  atl::string opToStr(const Op op) const;
};

}; // namespace ACC

#endif