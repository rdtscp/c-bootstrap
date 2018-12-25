#include "../../include/ast/CharLiteral.h"

using namespace ACC;

CharLiteral::CharLiteral(std::string literal) : value(literal) {}

void CharLiteral::accept(ASTVisitor<void> &v)  {
  return v.visit(*this);
}

std::string CharLiteral::accept(ASTVisitor<std::string> &v)  {
  return v.visit(*this);
}

std::shared_ptr<Type>
CharLiteral::accept(ASTVisitor<std::shared_ptr<Type>> &v)  {
  return v.visit(*this);
}

MIPS::Register CharLiteral::accept(ASTVisitor<MIPS::Register> &v)  {
  return v.visit(*this);
}

X86::Register CharLiteral::accept(ASTVisitor<X86::Register> &v)  {
  return v.visit(*this);
}

std::string CharLiteral::astClass() const  { return "CharLiteral"; }