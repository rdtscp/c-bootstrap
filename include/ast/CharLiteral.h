#pragma once

#ifndef CHARLITERAL_H
#define CHARLITERAL_H

#include <string>

#include "Expr.h"

namespace ACC {

class CharLiteral : public Expr {

public:
  std::string value;

  CharLiteral(std::string literal);

  void accept(ASTVisitor<void> &v) override;
  std::string accept(ASTVisitor<std::string> &v) override;
  std::shared_ptr<Type> accept(ASTVisitor<std::shared_ptr<Type>> &v) override;
  MIPS::Register accept(ASTVisitor<MIPS::Register> &v) override;
  X86::Register accept(ASTVisitor<X86::Register> &v) override;
  std::string astClass() const override;
};

}; // namespace ACC

#endif