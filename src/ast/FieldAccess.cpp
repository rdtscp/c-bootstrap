#include "../../include/ast/FieldAccess.h"

using namespace ACC;

FieldAccess::FieldAccess(std::shared_ptr<Expr> object, std::string field)
    : object(object), field(field) {}

void FieldAccess::accept(ASTVisitor<void> &v)  {
  return v.visit(*this);
}

std::string FieldAccess::accept(ASTVisitor<std::string> &v)  {
  return v.visit(*this);
}

std::shared_ptr<Type>
FieldAccess::accept(ASTVisitor<std::shared_ptr<Type>> &v)  {
  return v.visit(*this);
}

MIPS::Register FieldAccess::accept(ASTVisitor<MIPS::Register> &v)  {
  return v.visit(*this);
}

X86::Register FieldAccess::accept(ASTVisitor<X86::Register> &v)  {
  return v.visit(*this);
}

std::string FieldAccess::astClass() const  { return "FieldAccess"; }