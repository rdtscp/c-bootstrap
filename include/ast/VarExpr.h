#pragma once

#ifndef VAREXPR_H
#define VAREXPR_H

#include <string>

#include "Expr.h"

namespace ACC {

class VarExpr : public Expr {

public:
  std::string identifier;
  std::shared_ptr<VarDecl> variableDecl;

  VarExpr(std::string identifier);

  void accept(ASTVisitor<void> &v) override;
  std::string accept(ASTVisitor<std::string> &v) override;
  std::shared_ptr<Type> accept(ASTVisitor<std::shared_ptr<Type>> &v) override;
  MIPS::Register accept(ASTVisitor<MIPS::Register> &v) override;
  X86::Register accept(ASTVisitor<X86::Register> &v) override;
  std::string astClass() const override;
};

}; // namespace ACC

#endif