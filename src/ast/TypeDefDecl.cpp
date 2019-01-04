#include "../../include/ast/TypeDefDecl.h"

using namespace ACC;

TypeDefDecl::TypeDefDecl(std::shared_ptr<Type> type, std::string identifier)
    : type(type), identifier(identifier) {}

std::shared_ptr<TypeDefDecl::Decl> TypeDefDecl::getptr() { return shared_from_this(); }

void TypeDefDecl::accept(ASTVisitor<void> &v) { return v.visit(*this); }

std::string TypeDefDecl::accept(ASTVisitor<std::string> &v) {
  return v.visit(*this);
}

std::shared_ptr<Type> TypeDefDecl::accept(ASTVisitor<std::shared_ptr<Type>> &v) {
  return v.visit(*this);
}

MIPS::Register TypeDefDecl::accept(ASTVisitor<MIPS::Register> &v) {
  return v.visit(*this);
}

std::shared_ptr<X86::Operand>
TypeDefDecl::accept(ASTVisitor<std::shared_ptr<X86::Operand>> &v) {
  return v.visit(*this);
}

std::shared_ptr<ASTNode>
TypeDefDecl::accept(ASTVisitor<std::shared_ptr<ASTNode>> &v) {
  return v.visit(*this);
}

std::string TypeDefDecl::astClass() const { return "TypeDefDecl"; }