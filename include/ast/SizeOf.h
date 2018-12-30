#pragma once

#ifndef SIZEOF_H
#define SIZEOF_H

#include <string>

#include "Expr.h"
#include "Type.h"

namespace ACC {

class SizeOf : public Expr, public std::enable_shared_from_this<SizeOf> {

public:
  std::shared_ptr<Type> type;

  SizeOf(std::shared_ptr<Type> type);

  std::shared_ptr<SizeOf> getptr();

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