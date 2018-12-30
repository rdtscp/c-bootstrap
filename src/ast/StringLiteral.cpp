#include "../../include/ast/StringLiteral.h"

using namespace ACC;

StringLiteral::StringLiteral(std::string literal) : value(literal) {}

void StringLiteral::accept(ASTVisitor<void> &v) { return v.visit(*this); }

std::string StringLiteral::accept(ASTVisitor<std::string> &v) {
  return v.visit(*this);
}

std::shared_ptr<Type>
StringLiteral::accept(ASTVisitor<std::shared_ptr<Type>> &v) {
  return v.visit(*this);
}

MIPS::Register StringLiteral::accept(ASTVisitor<MIPS::Register> &v) {
  return v.visit(*this);
}

std::shared_ptr<X86::Operand> StringLiteral::accept(ASTVisitor<std::shared_ptr<X86::Operand>> &v) {
  return v.visit(*this);
}

std::string StringLiteral::astClass() const { return "StringLiteral"; }