#include "../../include/ast/SizeOf.h"

using namespace ACC;

SizeOf::SizeOf(std::shared_ptr<Type> type) : type(type) {}

void SizeOf::accept(ASTVisitor<void> &v)  { return v.visit(*this); }

std::string SizeOf::accept(ASTVisitor<std::string> &v)  {
  return v.visit(*this);
}

std::shared_ptr<Type>
SizeOf::accept(ASTVisitor<std::shared_ptr<Type>> &v)  {
  return v.visit(*this);
}

MIPS::Register SizeOf::accept(ASTVisitor<MIPS::Register> &v)  {
  return v.visit(*this);
}

std::shared_ptr<X86::Operand> SizeOf::accept(ASTVisitor<std::shared_ptr<X86::Operand>> &v)  {
  return v.visit(*this);
}

std::string SizeOf::astClass() const  { return "SizeOf"; }