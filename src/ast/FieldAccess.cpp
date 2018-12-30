#include "../../include/ast/FieldAccess.h"

using namespace ACC;

FieldAccess::FieldAccess(std::shared_ptr<Expr> object, std::string field)
    : object(object), field(field) {}

std::shared_ptr<FieldAccess> FieldAccess::getptr() {
  return shared_from_this();
}

void FieldAccess::accept(ASTVisitor<void> &v) { return v.visit(*this); }

std::string FieldAccess::accept(ASTVisitor<std::string> &v) {
  return v.visit(*this);
}

std::shared_ptr<Type>
FieldAccess::accept(ASTVisitor<std::shared_ptr<Type>> &v) {
  return v.visit(*this);
}

MIPS::Register FieldAccess::accept(ASTVisitor<MIPS::Register> &v) {
  return v.visit(*this);
}

std::shared_ptr<X86::Operand>
FieldAccess::accept(ASTVisitor<std::shared_ptr<X86::Operand>> &v) {
  return v.visit(*this);
}

std::shared_ptr<ASTNode>
FieldAccess::accept(ASTVisitor<std::shared_ptr<ASTNode>> &v) {
  return v.visit(*this);
}

std::string FieldAccess::astClass() const { return "FieldAccess"; }