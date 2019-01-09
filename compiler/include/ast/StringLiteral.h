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
  std::string value;

public:
  StringLiteral(std::string literal);

  std::shared_ptr<StringLiteral> getptr();

  std::string getLiteral() const override;

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