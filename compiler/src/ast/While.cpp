#include "../../include/ast/While.h"

using namespace ACC;

While::While(std::shared_ptr<Stmt> body, std::shared_ptr<Expr> condition)
    : body(body), condition(condition) {}

std::shared_ptr<While> While::getptr() { return shared_from_this(); }

void While::accept(ASTVisitor<void> &v) { return v.visit(*this); }

atl::string While::accept(ASTVisitor<atl::string> &v) { return v.visit(*this); }

std::shared_ptr<Type> While::accept(ASTVisitor<std::shared_ptr<Type>> &v) {
  return v.visit(*this);
}

MIPS::Register While::accept(ASTVisitor<MIPS::Register> &v) {
  return v.visit(*this);
}

std::shared_ptr<X86::Operand>
While::accept(ASTVisitor<std::shared_ptr<X86::Operand>> &v) {
  return v.visit(*this);
}

std::shared_ptr<ASTNode>
While::accept(ASTVisitor<std::shared_ptr<ASTNode>> &v) {
  return v.visit(*this);
}

atl::string While::astClass() const { return "While"; }