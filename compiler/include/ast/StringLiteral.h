#pragma once

#ifndef ACC_STRINGLITERAL_H
#define ACC_STRINGLITERAL_H

#include <string>

#include "Expr.h"
#include "Literal.h"

namespace ACC {

class StringLiteral : public Literal,
                      public std::enable_shared_from_this<StringLiteral> {
private:
  const atl::string value;

public:
  StringLiteral(const atl::string &literal);

  std::shared_ptr<StringLiteral> getptr();

  atl::string getLiteral() const override;

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