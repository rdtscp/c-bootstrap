#include "../../include/ast/ValueAt.h"

using namespace ACC;

ValueAt::ValueAt(std::shared_ptr<Expr> derefExpr) : derefExpr(derefExpr) {}

void ValueAt::accept(ASTVisitor<void> &v)  { return v.visit(*this); }

std::string ValueAt::accept(ASTVisitor<std::string> &v)  {
  return v.visit(*this);
}

std::shared_ptr<Type>
ValueAt::accept(ASTVisitor<std::shared_ptr<Type>> &v)  {
  return v.visit(*this);
}

MIPS::Register ValueAt::accept(ASTVisitor<MIPS::Register> &v)  {
  return v.visit(*this);
}

X86::Register ValueAt::accept(ASTVisitor<X86::Register> &v)  {
  return v.visit(*this);
}

std::string ValueAt::astClass() const  { return "ValueAt"; }