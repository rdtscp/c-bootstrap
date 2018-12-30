#include "../../include/ast/ParenthExpr.h"

using namespace ACC;

ParenthExpr::ParenthExpr(std::shared_ptr<Expr> innerExpr)
    : innerExpr(innerExpr) {}

void ParenthExpr::accept(ASTVisitor<void> &v)  {
  return v.visit(*this);
}

std::string ParenthExpr::accept(ASTVisitor<std::string> &v)  {
  return v.visit(*this);
}

std::shared_ptr<Type>
ParenthExpr::accept(ASTVisitor<std::shared_ptr<Type>> &v)  {
  return v.visit(*this);
}

MIPS::Register ParenthExpr::accept(ASTVisitor<MIPS::Register> &v)  {
  return v.visit(*this);
}

std::shared_ptr<X86::Operand> ParenthExpr::accept(ASTVisitor<std::shared_ptr<X86::Operand>> &v)  {
  return v.visit(*this);
}

std::string ParenthExpr::astClass() const  { return "ParenthExpr"; }
