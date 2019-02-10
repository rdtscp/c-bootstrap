#include "../../include/ast/DoWhile.h"

using namespace ACC;

DoWhile::DoWhile(atl::shared_ptr<Stmt> body, atl::shared_ptr<Expr> condition)
    : body(body), condition(condition) {}

atl::shared_ptr<DoWhile> DoWhile::getptr() { return shared_from_this(); }

void DoWhile::accept(ASTVisitor<void> &v) { return v.visit(*this); }

atl::string DoWhile::accept(ASTVisitor<atl::string> &v) {
  return v.visit(*this);
}

atl::shared_ptr<Type> DoWhile::accept(ASTVisitor<atl::shared_ptr<Type>> &v) {
  return v.visit(*this);
}

MIPS::Register DoWhile::accept(ASTVisitor<MIPS::Register> &v) {
  return v.visit(*this);
}

atl::shared_ptr<X86::Operand>
DoWhile::accept(ASTVisitor<atl::shared_ptr<X86::Operand>> &v) {
  return v.visit(*this);
}

atl::shared_ptr<ASTNode>
DoWhile::accept(ASTVisitor<atl::shared_ptr<ASTNode>> &v) {
  return v.visit(*this);
}

atl::string DoWhile::astClass() const { return "DoWhile"; }