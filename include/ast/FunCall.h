#pragma once

#ifndef FUNCALL_H
#define FUNCALL_H

#include <string>
#include <vector>

#include "Expr.h"

namespace ACC {

class FunCall : public Expr, public std::enable_shared_from_this<FunCall> {

public:
  std::string funName;
  std::vector<std::shared_ptr<Expr>> funArgs;

  FunCall(std::string funName, std::vector<std::shared_ptr<Expr>> funArgs);

  std::shared_ptr<FunCall> getptr();

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