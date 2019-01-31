#include "../../include/ast/Namespace.h"

using namespace ACC;

Namespace::Namespace(const std::string &identifier,
                     const std::shared_ptr<Block> &namespaceBlock) {}

void Namespace::accept(ASTVisitor<void> &v) { return v.visit(*this); }

std::shared_ptr<Namespace> Namespace::getptr() { return shared_from_this(); }

atl::string Namespace::accept(ASTVisitor<atl::string> &v) {
  return v.visit(*this);
}

std::shared_ptr<Type> Namespace::accept(ASTVisitor<std::shared_ptr<Type>> &v) {
  return v.visit(*this);
}

MIPS::Register Namespace::accept(ASTVisitor<MIPS::Register> &v) {
  return v.visit(*this);
}

std::shared_ptr<X86::Operand>
Namespace::accept(ASTVisitor<std::shared_ptr<X86::Operand>> &v) {
  return v.visit(*this);
}

std::shared_ptr<ASTNode>
Namespace::accept(ASTVisitor<std::shared_ptr<ASTNode>> &v) {
  return v.visit(*this);
}

atl::string Namespace::astClass() const { return "Program"; }