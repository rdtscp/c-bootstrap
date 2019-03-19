#include "../../include/ast/Return.h"

using namespace ACC;

Return::Return() : returnExpr(nullptr) {}

Return::Return(const atl::shared_ptr<Expr> &returnExpr)
    : returnExpr(returnExpr) {}

atl::shared_ptr<Return> Return::getptr() { return shared_from_this(); }

void Return::accept(ASTVisitor<void> &v) { return v.visit(*this); }

atl::string Return::accept(ASTVisitor<atl::string> &v) {
  return v.visit(*this);
}

atl::shared_ptr<Type> Return::accept(ASTVisitor<atl::shared_ptr<Type>> &v) {
  return v.visit(*this);
}

MIPS::Register Return::accept(ASTVisitor<MIPS::Register> &v) {
  return v.visit(*this);
}

atl::shared_ptr<X86::Operand>
Return::accept(ASTVisitor<atl::shared_ptr<X86::Operand>> &v) {
  return v.visit(*this);
}

atl::shared_ptr<ASTNode>
Return::accept(ASTVisitor<atl::shared_ptr<ASTNode>> &v) {
  return v.visit(*this);
}

atl::string Return::astClass() const { return "Return"; }