#pragma once

#ifndef ACC_IF_H
#define ACC_IF_H

#include "Expr.h"

namespace ACC {

class If : public Stmt, public atl::enable_shared_from_this<If> {

public:
  atl::shared_ptr<Expr> ifCondition;
  atl::shared_ptr<Stmt> ifBody;
  atl::shared_ptr<Stmt> elseBody;

  If(const atl::shared_ptr<Expr> &ifCondition,
     const atl::shared_ptr<Stmt> &ifBody);

  If(const atl::shared_ptr<Expr> &ifCondition,
     const atl::shared_ptr<Stmt> &ifBody,
     const atl::shared_ptr<Stmt> &elseBody);

  atl::shared_ptr<If> getptr();

  void accept(ASTVisitor<void> &v) override;
  atl::string accept(ASTVisitor<atl::string> &v) override;
  atl::shared_ptr<Type> accept(ASTVisitor<atl::shared_ptr<Type>> &v) override;
  MIPS::Register accept(ASTVisitor<MIPS::Register> &v) override;
  atl::shared_ptr<X86::Operand>
  accept(ASTVisitor<atl::shared_ptr<X86::Operand>> &v) override;
  atl::shared_ptr<ASTNode>
  accept(ASTVisitor<atl::shared_ptr<ASTNode>> &v) override;
  atl::string astClass() const override;
};

}; // namespace ACC

#endif