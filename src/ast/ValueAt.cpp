#include "../../include/ast/ValueAt.h"

using namespace ACC;

ValueAt::ValueAt(std::shared_ptr<Expr> derefExpr) : derefExpr(derefExpr) {}

std::shared_ptr<ValueAt> ValueAt::getptr() { return shared_from_this(); }

void ValueAt::accept(ASTVisitor<void> &v) { return v.visit(*this); }

std::string ValueAt::accept(ASTVisitor<std::string> &v) {
  return v.visit(*this);
}

std::shared_ptr<Type> ValueAt::accept(ASTVisitor<std::shared_ptr<Type>> &v) {
  return v.visit(*this);
}

MIPS::Register ValueAt::accept(ASTVisitor<MIPS::Register> &v) {
  return v.visit(*this);
}

std::shared_ptr<X86::Operand>
ValueAt::accept(ASTVisitor<std::shared_ptr<X86::Operand>> &v) {
  return v.visit(*this);
}

std::shared_ptr<ASTNode>
ValueAt::accept(ASTVisitor<std::shared_ptr<ASTNode>> &v) {
  return v.visit(*this);
}

std::string ValueAt::astClass() const { return "ValueAt"; }