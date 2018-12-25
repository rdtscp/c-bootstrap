#include "../../include/ast/While.h"

using namespace ACC;

While::While(std::shared_ptr<Stmt> body, std::shared_ptr<Expr> condition)
    : body(body), condition(condition) {}

void While::accept(ASTVisitor<void> &v)  { return v.visit(*this); }

std::string While::accept(ASTVisitor<std::string> &v)  {
  return v.visit(*this);
}

std::shared_ptr<Type>
While::accept(ASTVisitor<std::shared_ptr<Type>> &v)  {
  return v.visit(*this);
}

MIPS::Register While::accept(ASTVisitor<MIPS::Register> &v)  {
  return v.visit(*this);
}

X86::Register While::accept(ASTVisitor<X86::Register> &v)  {
  return v.visit(*this);
}

std::string While::astClass() const  { return "While"; }