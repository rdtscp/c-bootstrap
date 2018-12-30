#pragma once

#ifndef STRINGLITERAL_H
#define STRINGLITERAL_H

#include <string>

#include "Expr.h"

namespace ACC {

class StringLiteral : public Expr,
                      public std::enable_shared_from_this<StringLiteral> {

public:
  std::string value;

  StringLiteral(std::string literal);

  std::shared_ptr<StringLiteral> getptr();

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