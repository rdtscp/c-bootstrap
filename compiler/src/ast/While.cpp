#include "../../include/ast/While.h"

using namespace ACC;

While::While(const atl::shared_ptr<Stmt> &p_body,
             const atl::shared_ptr<Expr> &p_condition)
    : body(p_body), condition(p_condition) {}

atl::shared_ptr<While> While::getptr() { return shared_from_this(); }

void While::accept(ASTVisitor<void> &v) { return v.visit(*this); }

atl::string While::accept(ASTVisitor<atl::string> &v) { return v.visit(*this); }

atl::shared_ptr<Type> While::accept(ASTVisitor<atl::shared_ptr<Type>> &v) {
  return v.visit(*this);
}

MIPS::Register While::accept(ASTVisitor<MIPS::Register> &v) {
  return v.visit(*this);
}

atl::shared_ptr<X86::Operand>
While::accept(ASTVisitor<atl::shared_ptr<X86::Operand>> &v) {
  return v.visit(*this);
}

atl::shared_ptr<ASTNode>
While::accept(ASTVisitor<atl::shared_ptr<ASTNode>> &v) {
  return v.visit(*this);
}

atl::string While::astClass() const { return "While"; }