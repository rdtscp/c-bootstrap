#include "../../include/ast/Namespace.h"

using namespace ACC;

Namespace::Namespace(const atl::shared_ptr<Identifier> &identifier,
                     const atl::vector<atl::shared_ptr<Decl>> &namespaceDecls)
    : identifier(identifier), namespaceDecls(namespaceDecls) {}

void Namespace::accept(ASTVisitor<void> &v) { return v.visit(*this); }

atl::shared_ptr<Namespace> Namespace::getptr() { return shared_from_this(); }

bool Namespace::operator==(Decl &rhs) const {
  if (rhs.astClass() == astClass())
    return *this == *static_cast<Namespace *>(&rhs);
  return false;
}

bool Namespace::operator!=(Decl &rhs) const { return !(*this == rhs); }

atl::shared_ptr<Identifier> Namespace::getIdentifier() const {
  return identifier;
}

atl::string Namespace::accept(ASTVisitor<atl::string> &v) {
  return v.visit(*this);
}

atl::shared_ptr<Type> Namespace::accept(ASTVisitor<atl::shared_ptr<Type>> &v) {
  return v.visit(*this);
}

MIPS::Register Namespace::accept(ASTVisitor<MIPS::Register> &v) {
  return v.visit(*this);
}

atl::shared_ptr<X86::Operand>
Namespace::accept(ASTVisitor<atl::shared_ptr<X86::Operand>> &v) {
  return v.visit(*this);
}

atl::shared_ptr<ASTNode>
Namespace::accept(ASTVisitor<atl::shared_ptr<ASTNode>> &v) {
  return v.visit(*this);
}

atl::string Namespace::astClass() const { return "Program"; }