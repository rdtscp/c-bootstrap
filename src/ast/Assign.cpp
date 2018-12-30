#include "../../include/ast/Assign.h"

using namespace ACC;

Assign::Assign(std::shared_ptr<Expr> lhs, std::shared_ptr<Expr> rhs)
    : lhs(lhs), rhs(rhs) {}

void Assign::accept(ASTVisitor<void> &v) { return v.visit(*this); }

std::string Assign::accept(ASTVisitor<std::string> &v) {
  return v.visit(*this);
}

std::shared_ptr<Type> Assign::accept(ASTVisitor<std::shared_ptr<Type>> &v) {
  return v.visit(*this);
}

MIPS::Register Assign::accept(ASTVisitor<MIPS::Register> &v) {
  return v.visit(*this);
}

std::shared_ptr<X86::Operand> Assign::accept(ASTVisitor<std::shared_ptr<X86::Operand>> &v) {
  return v.visit(*this);
}

std::string Assign::astClass() const { return "Assign"; }