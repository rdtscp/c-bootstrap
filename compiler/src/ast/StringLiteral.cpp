#include "../../include/ast/StringLiteral.h"

using namespace ACC;

StringLiteral::StringLiteral(const atl::string &literal) : value(literal) {}

atl::shared_ptr<StringLiteral> StringLiteral::getptr() {
  return shared_from_this();
}

atl::string StringLiteral::getLiteral() const { return value; }

void StringLiteral::accept(ASTVisitor<void> &v) { return v.visit(*this); }

atl::string StringLiteral::accept(ASTVisitor<atl::string> &v) {
  return v.visit(*this);
}

atl::shared_ptr<Type>
StringLiteral::accept(ASTVisitor<atl::shared_ptr<Type>> &v) {
  return v.visit(*this);
}

MIPS::Register StringLiteral::accept(ASTVisitor<MIPS::Register> &v) {
  return v.visit(*this);
}

atl::shared_ptr<X86::Operand>
StringLiteral::accept(ASTVisitor<atl::shared_ptr<X86::Operand>> &v) {
  return v.visit(*this);
}

atl::shared_ptr<ASTNode>
StringLiteral::accept(ASTVisitor<atl::shared_ptr<ASTNode>> &v) {
  return v.visit(*this);
}

atl::string StringLiteral::astClass() const { return "StringLiteral"; }