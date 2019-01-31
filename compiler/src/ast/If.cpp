#include "../../include/ast/If.h"

using namespace ACC;

If::If(std::shared_ptr<Expr> ifCondition, std::shared_ptr<Stmt> ifBody)
    : ifBody(ifBody), ifCondition(ifCondition) {}

If::If(std::shared_ptr<Expr> ifCondition, std::shared_ptr<Stmt> ifBody,
       std::shared_ptr<Stmt> elseBody)
    : ifBody(ifBody), ifCondition(ifCondition), elseBody(elseBody) {}

std::shared_ptr<If> If::getptr() { return shared_from_this(); }

void If::accept(ASTVisitor<void> &v) { return v.visit(*this); }

atl::string If::accept(ASTVisitor<atl::string> &v) { return v.visit(*this); }

std::shared_ptr<Type> If::accept(ASTVisitor<std::shared_ptr<Type>> &v) {
  return v.visit(*this);
}

MIPS::Register If::accept(ASTVisitor<MIPS::Register> &v) {
  return v.visit(*this);
}

std::shared_ptr<X86::Operand>
If::accept(ASTVisitor<std::shared_ptr<X86::Operand>> &v) {
  return v.visit(*this);
}

std::shared_ptr<ASTNode> If::accept(ASTVisitor<std::shared_ptr<ASTNode>> &v) {
  return v.visit(*this);
}

atl::string If::astClass() const { return "If"; }