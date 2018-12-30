#include "../../include/ast/TypeCast.h"

using namespace ACC;

TypeCast::TypeCast(std::shared_ptr<Type> type, std::shared_ptr<Expr> expr)
    : type(type), expr(expr) {}

std::shared_ptr<TypeCast> TypeCast::getptr() { return shared_from_this(); }

void TypeCast::accept(ASTVisitor<void> &v) { return v.visit(*this); }

std::string TypeCast::accept(ASTVisitor<std::string> &v) {
  return v.visit(*this);
}

std::shared_ptr<Type> TypeCast::accept(ASTVisitor<std::shared_ptr<Type>> &v) {
  return v.visit(*this);
}

MIPS::Register TypeCast::accept(ASTVisitor<MIPS::Register> &v) {
  return v.visit(*this);
}

std::shared_ptr<X86::Operand>
TypeCast::accept(ASTVisitor<std::shared_ptr<X86::Operand>> &v) {
  return v.visit(*this);
}

std::shared_ptr<ASTNode>
TypeCast::accept(ASTVisitor<std::shared_ptr<ASTNode>> &v) {
  return v.visit(*this);
}

std::string TypeCast::astClass() const { return "TypeCast"; }