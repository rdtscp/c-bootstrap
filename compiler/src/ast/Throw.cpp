#include "../../include/ast/Throw.h"

using namespace ACC;

Throw::Throw(const atl::shared_ptr<StringLiteral> &exceptionText)
    : exceptionText(exceptionText) {}

atl::shared_ptr<Throw> Throw::getptr() { return shared_from_this(); }

void Throw::accept(ASTVisitor<void> &v) { return v.visit(*this); }

atl::string Throw::accept(ASTVisitor<atl::string> &v) { return v.visit(*this); }

atl::shared_ptr<Type> Throw::accept(ASTVisitor<atl::shared_ptr<Type>> &v) {
  return v.visit(*this);
}

MIPS::Register Throw::accept(ASTVisitor<MIPS::Register> &v) {
  return v.visit(*this);
}

atl::shared_ptr<X86::Operand>
Throw::accept(ASTVisitor<atl::shared_ptr<X86::Operand>> &v) {
  return v.visit(*this);
}

atl::shared_ptr<ASTNode>
Throw::accept(ASTVisitor<atl::shared_ptr<ASTNode>> &v) {
  return v.visit(*this);
}

atl::string Throw::astClass() const { return "Throw"; }