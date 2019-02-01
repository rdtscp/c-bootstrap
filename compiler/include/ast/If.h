#pragma once

#ifndef ACC_IF_H
#define ACC_IF_H

#include "Expr.h"

namespace ACC {

class If : public Stmt, public std::enable_shared_from_this<If> {

public:
  std::shared_ptr<Expr> ifCondition;
  std::shared_ptr<Stmt> ifBody;
  std::shared_ptr<Stmt> elseBody = nullptr;

  If(std::shared_ptr<Expr> ifCondition, std::shared_ptr<Stmt> ifBody);

  If(std::shared_ptr<Expr> ifCondition, std::shared_ptr<Stmt> ifBody,
     std::shared_ptr<Stmt> elseBody);

  std::shared_ptr<If> getptr();

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