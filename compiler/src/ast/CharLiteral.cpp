#include "../../include/ast/CharLiteral.h"

using namespace ACC;

CharLiteral::CharLiteral(const atl::string &literal) : value(literal) {}

std::shared_ptr<CharLiteral> CharLiteral::getptr() {
  return shared_from_this();
}

atl::string CharLiteral::getLiteral() const { return value; }

void CharLiteral::accept(ASTVisitor<void> &v) { return v.visit(*this); }

atl::string CharLiteral::accept(ASTVisitor<atl::string> &v) {
  return v.visit(*this);
}

std::shared_ptr<Type>
CharLiteral::accept(ASTVisitor<std::shared_ptr<Type>> &v) {
  return v.visit(*this);
}

MIPS::Register CharLiteral::accept(ASTVisitor<MIPS::Register> &v) {
  return v.visit(*this);
}

std::shared_ptr<X86::Operand>
CharLiteral::accept(ASTVisitor<std::shared_ptr<X86::Operand>> &v) {
  return v.visit(*this);
}

std::shared_ptr<ASTNode>
CharLiteral::accept(ASTVisitor<std::shared_ptr<ASTNode>> &v) {
  return v.visit(*this);
}

atl::string CharLiteral::astClass() const { return "CharLiteral"; }