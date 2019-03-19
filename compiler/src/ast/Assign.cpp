#include "../../include/ast/Assign.h"

using namespace ACC;

Assign::Assign(const atl::shared_ptr<Expr> &p_lhs,
               const atl::shared_ptr<Expr> &p_rhs)
    : lhs(p_lhs), rhs(p_rhs) {}

atl::shared_ptr<Assign> Assign::getptr() { return shared_from_this(); }

void Assign::accept(ASTVisitor<void> &v) { return v.visit(*this); }

atl::string Assign::accept(ASTVisitor<atl::string> &v) {
  return v.visit(*this);
}

atl::shared_ptr<Type> Assign::accept(ASTVisitor<atl::shared_ptr<Type>> &v) {
  return v.visit(*this);
}

MIPS::Register Assign::accept(ASTVisitor<MIPS::Register> &v) {
  return v.visit(*this);
}

atl::shared_ptr<X86::Operand>
Assign::accept(ASTVisitor<atl::shared_ptr<X86::Operand>> &v) {
  return v.visit(*this);
}

atl::shared_ptr<ASTNode>
Assign::accept(ASTVisitor<atl::shared_ptr<ASTNode>> &v) {
  return v.visit(*this);
}

atl::string Assign::astClass() const { return "Assign"; }