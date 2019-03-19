#include "../../include/ast/If.h"

using namespace ACC;

If::If(const atl::shared_ptr<Expr> &p_ifCondition,
       const atl::shared_ptr<Stmt> &p_ifBody)
    : ifBody(p_ifBody), ifCondition(p_ifCondition), elseBody(nullptr) {}

If::If(const atl::shared_ptr<Expr> &p_ifCondition,
       const atl::shared_ptr<Stmt> &p_ifBody,
       const atl::shared_ptr<Stmt> &p_elseBody)
    : ifBody(p_ifBody), ifCondition(p_ifCondition), elseBody(p_elseBody) {}

atl::shared_ptr<If> If::getptr() { return shared_from_this(); }

void If::accept(ASTVisitor<void> &v) { return v.visit(*this); }

atl::string If::accept(ASTVisitor<atl::string> &v) { return v.visit(*this); }

atl::shared_ptr<Type> If::accept(ASTVisitor<atl::shared_ptr<Type>> &v) {
  return v.visit(*this);
}

MIPS::Register If::accept(ASTVisitor<MIPS::Register> &v) {
  return v.visit(*this);
}

atl::shared_ptr<X86::Operand>
If::accept(ASTVisitor<atl::shared_ptr<X86::Operand>> &v) {
  return v.visit(*this);
}

atl::shared_ptr<ASTNode> If::accept(ASTVisitor<atl::shared_ptr<ASTNode>> &v) {
  return v.visit(*this);
}

atl::string If::astClass() const { return "If"; }