#pragma once

#ifndef IF_H
#define IF_H

#include <string>

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