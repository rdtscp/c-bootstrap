#pragma once

#ifndef ACC_CHARLITERAL_H
#define ACC_CHARLITERAL_H

#include <string>

#include "Expr.h"
#include "Literal.h"

namespace ACC {

class CharLiteral : public Literal,
                    public std::enable_shared_from_this<CharLiteral> {
private:
  const atl::string value;

public:
  CharLiteral(const atl::string &literal);

  std::shared_ptr<CharLiteral> getptr();

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