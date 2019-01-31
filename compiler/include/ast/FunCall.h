#pragma once

#ifndef ACC_FUNCALL_H
#define ACC_FUNCALL_H

#include <string>
#include <vector>

#include "Expr.h"

namespace ACC {

class FunCall : public Expr, public std::enable_shared_from_this<FunCall> {

public:
  atl::string funName;
  std::vector<std::shared_ptr<Expr>> funArgs;

  FunCall(const atl::string &funName,
          std::vector<std::shared_ptr<Expr>> funArgs);

  std::shared_ptr<FunCall> getptr();

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