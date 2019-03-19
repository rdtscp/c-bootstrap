#pragma once

#ifndef ACC_BOOLLITERAL_H
#define ACC_BOOLLITERAL_H

#include "Expr.h"
#include "Literal.h"

namespace ACC {

class BoolLiteral : public Literal,
                    public atl::enable_shared_from_this<BoolLiteral> {
public:
  BoolLiteral(const atl::string &p_literal);

  atl::shared_ptr<BoolLiteral> getptr();

  atl::string getLiteral() const override;

  bool operator==(Expr &rhs) const override;
  bool operator!=(Expr &rhs) const override;

  bool operator==(Literal &rhs) const override;
  bool operator!=(Literal &rhs) const override;

  bool operator==(const BoolLiteral &rhs) const;
  bool operator!=(const BoolLiteral &rhs) const;

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