#include "../../include/ast/Allocation.h"

using namespace ACC;

Allocation::Allocation(const atl::shared_ptr<Type> &variableType)
    : variableType(variableType), variableConstructorCall(nullptr) {}

Allocation::Allocation(const atl::shared_ptr<FunCall> &variableConstructorCall)
    : variableType(nullptr), variableConstructorCall(variableConstructorCall) {}

atl::shared_ptr<Allocation> Allocation::getptr() { return shared_from_this(); }

void Allocation::accept(ASTVisitor<void> &v) { return v.visit(*this); }

atl::string Allocation::accept(ASTVisitor<atl::string> &v) {
  return v.visit(*this);
}

atl::shared_ptr<Type> Allocation::accept(ASTVisitor<atl::shared_ptr<Type>> &v) {
  return v.visit(*this);
}

MIPS::Register Allocation::accept(ASTVisitor<MIPS::Register> &v) {
  return v.visit(*this);
}

atl::shared_ptr<X86::Operand>
Allocation::accept(ASTVisitor<atl::shared_ptr<X86::Operand>> &v) {
  return v.visit(*this);
}

atl::shared_ptr<ASTNode>
Allocation::accept(ASTVisitor<atl::shared_ptr<ASTNode>> &v) {
  return v.visit(*this);
}

atl::string Allocation::astClass() const { return "Allocation"; }