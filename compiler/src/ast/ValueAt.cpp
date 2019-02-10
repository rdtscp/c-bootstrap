#include "../../include/ast/ValueAt.h"

using namespace ACC;

ValueAt::ValueAt(atl::shared_ptr<Expr> derefExpr) : derefExpr(derefExpr) {}

atl::shared_ptr<ValueAt> ValueAt::getptr() { return shared_from_this(); }

void ValueAt::accept(ASTVisitor<void> &v) { return v.visit(*this); }

atl::string ValueAt::accept(ASTVisitor<atl::string> &v) {
  return v.visit(*this);
}

atl::shared_ptr<Type> ValueAt::accept(ASTVisitor<atl::shared_ptr<Type>> &v) {
  return v.visit(*this);
}

MIPS::Register ValueAt::accept(ASTVisitor<MIPS::Register> &v) {
  return v.visit(*this);
}

atl::shared_ptr<X86::Operand>
ValueAt::accept(ASTVisitor<atl::shared_ptr<X86::Operand>> &v) {
  return v.visit(*this);
}

atl::shared_ptr<ASTNode>
ValueAt::accept(ASTVisitor<atl::shared_ptr<ASTNode>> &v) {
  return v.visit(*this);
}

atl::string ValueAt::astClass() const { return "ValueAt"; }