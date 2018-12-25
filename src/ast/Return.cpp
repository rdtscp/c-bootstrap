#include "../../include/ast/Return.h"

using namespace ACC;

Return::Return() {}

Return::Return(std::shared_ptr<Expr> returnExpr) : returnExpr(returnExpr) {}

void Return::accept(ASTVisitor<void> &v)  { return v.visit(*this); }

std::string Return::accept(ASTVisitor<std::string> &v)  {
  return v.visit(*this);
}

std::shared_ptr<Type>
Return::accept(ASTVisitor<std::shared_ptr<Type>> &v)  {
  return v.visit(*this);
}

MIPS::Register Return::accept(ASTVisitor<MIPS::Register> &v)  {
  return v.visit(*this);
}

X86::Register Return::accept(ASTVisitor<X86::Register> &v)  {
  return v.visit(*this);
}

std::string Return::astClass() const  { return "Return"; }