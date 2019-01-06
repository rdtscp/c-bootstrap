#include "../../include/ast/DoWhile.h"

using namespace ACC;

DoWhile::DoWhile(std::shared_ptr<Stmt> body, std::shared_ptr<Expr> condition)
    : body(body), condition(condition) {}

std::shared_ptr<DoWhile> DoWhile::getptr() { return shared_from_this(); }

void DoWhile::accept(ASTVisitor<void> &v) { return v.visit(*this); }

std::string DoWhile::accept(ASTVisitor<std::string> &v) {
  return v.visit(*this);
}

std::shared_ptr<Type> DoWhile::accept(ASTVisitor<std::shared_ptr<Type>> &v) {
  return v.visit(*this);
}

MIPS::Register DoWhile::accept(ASTVisitor<MIPS::Register> &v) {
  return v.visit(*this);
}

std::shared_ptr<X86::Operand>
DoWhile::accept(ASTVisitor<std::shared_ptr<X86::Operand>> &v) {
  return v.visit(*this);
}

std::shared_ptr<ASTNode>
DoWhile::accept(ASTVisitor<std::shared_ptr<ASTNode>> &v) {
  return v.visit(*this);
}

std::string DoWhile::astClass() const { return "DoWhile"; }