#include "../../include/ast/IntLiteral.h"

using namespace ACC;

IntLiteral::IntLiteral(std::string literal) : value(literal) {}

void IntLiteral::accept(ASTVisitor<void> &v) { return v.visit(*this); }

std::string IntLiteral::accept(ASTVisitor<std::string> &v) {
  return v.visit(*this);
}

std::shared_ptr<Type> IntLiteral::accept(ASTVisitor<std::shared_ptr<Type>> &v) {
  return v.visit(*this);
}

MIPS::Register IntLiteral::accept(ASTVisitor<MIPS::Register> &v) {
  return v.visit(*this);
}

std::shared_ptr<X86::Operand> IntLiteral::accept(ASTVisitor<std::shared_ptr<X86::Operand>> &v) {
  return v.visit(*this);
}

std::string IntLiteral::astClass() const { return "IntLiteral"; }