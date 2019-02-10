#include "../../include/ast/TypeCast.h"

using namespace ACC;

TypeCast::TypeCast(atl::shared_ptr<Type> type, atl::shared_ptr<Expr> expr)
    : type(type), expr(expr) {}

atl::shared_ptr<TypeCast> TypeCast::getptr() { return shared_from_this(); }

void TypeCast::accept(ASTVisitor<void> &v) { return v.visit(*this); }

atl::string TypeCast::accept(ASTVisitor<atl::string> &v) {
  return v.visit(*this);
}

atl::shared_ptr<Type> TypeCast::accept(ASTVisitor<atl::shared_ptr<Type>> &v) {
  return v.visit(*this);
}

MIPS::Register TypeCast::accept(ASTVisitor<MIPS::Register> &v) {
  return v.visit(*this);
}

atl::shared_ptr<X86::Operand>
TypeCast::accept(ASTVisitor<atl::shared_ptr<X86::Operand>> &v) {
  return v.visit(*this);
}

atl::shared_ptr<ASTNode>
TypeCast::accept(ASTVisitor<atl::shared_ptr<ASTNode>> &v) {
  return v.visit(*this);
}

atl::string TypeCast::astClass() const { return "TypeCast"; }