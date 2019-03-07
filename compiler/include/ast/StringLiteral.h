#pragma once

#ifndef ACC_STRINGLITERAL_H
#define ACC_STRINGLITERAL_H

#include "Expr.h"
#include "Literal.h"

namespace ACC {

class StringLiteral : public Literal,
                      public atl::enable_shared_from_this<StringLiteral> {
private:
  const atl::string value;

public:
  StringLiteral(const atl::string &literal);

  atl::shared_ptr<StringLiteral> getptr();

  atl::string getLiteral() const override;

  bool operator==(Expr &rhs) const override;
  bool operator!=(Expr &rhs) const override;

  bool operator==(Literal &rhs) const override;
  bool operator!=(Literal &rhs) const override;

  bool operator==(const StringLiteral &rhs) const;
  bool operator!=(const StringLiteral &rhs) const;

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