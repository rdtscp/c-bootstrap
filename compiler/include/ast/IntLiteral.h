#pragma once

#ifndef ACC_INTLITERAL_H
#define ACC_INTLITERAL_H

#include "Expr.h"
#include "Literal.h"

namespace ACC {

class IntLiteral : public Literal,
                   public atl::enable_shared_from_this<IntLiteral> {
private:
  const atl::string value;

public:
  IntLiteral(const atl::string &literal);

  atl::shared_ptr<IntLiteral> getptr();

  atl::string getLiteral() const override;

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