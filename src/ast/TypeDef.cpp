#include "../../include/ast/TypeDef.h"

using namespace ACC;

TypeDef::TypeDef(std::shared_ptr<Type> type, std::string identifier)
    : type(type), identifier(identifier) {}

std::shared_ptr<TypeDef::Decl> TypeDef::getptr() { return shared_from_this(); }

void TypeDef::accept(ASTVisitor<void> &v) { return v.visit(*this); }

std::string TypeDef::accept(ASTVisitor<std::string> &v) {
  return v.visit(*this);
}

std::shared_ptr<Type> TypeDef::accept(ASTVisitor<std::shared_ptr<Type>> &v) {
  return v.visit(*this);
}

MIPS::Register TypeDef::accept(ASTVisitor<MIPS::Register> &v) {
  return v.visit(*this);
}

std::shared_ptr<X86::Operand>
TypeDef::accept(ASTVisitor<std::shared_ptr<X86::Operand>> &v) {
  return v.visit(*this);
}

std::shared_ptr<ASTNode>
TypeDef::accept(ASTVisitor<std::shared_ptr<ASTNode>> &v) {
  return v.visit(*this);
}

std::string TypeDef::astClass() const { return "TypeDef"; }