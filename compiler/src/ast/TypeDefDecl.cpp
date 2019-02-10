#include "../../include/ast/TypeDefDecl.h"

using namespace ACC;

TypeDefDecl::TypeDefDecl(atl::shared_ptr<Type> type,
                         const atl::string &identifier)
    : type(type), identifier(identifier) {}

atl::shared_ptr<TypeDefDecl> TypeDefDecl::getptr() {
  return shared_from_this();
}

bool TypeDefDecl::operator==(Decl &rhs) const {
  if (rhs.astClass() == astClass())
    return *this == *static_cast<TypeDefDecl *>(&rhs);
  return false;
}

bool TypeDefDecl::operator!=(Decl &rhs) const { return !(*this == rhs); }

atl::string TypeDefDecl::getIdentifier() const { return identifier; }

void TypeDefDecl::accept(ASTVisitor<void> &v) { return v.visit(*this); }

atl::string TypeDefDecl::accept(ASTVisitor<atl::string> &v) {
  return v.visit(*this);
}

atl::shared_ptr<Type>
TypeDefDecl::accept(ASTVisitor<atl::shared_ptr<Type>> &v) {
  return v.visit(*this);
}

MIPS::Register TypeDefDecl::accept(ASTVisitor<MIPS::Register> &v) {
  return v.visit(*this);
}

atl::shared_ptr<X86::Operand>
TypeDefDecl::accept(ASTVisitor<atl::shared_ptr<X86::Operand>> &v) {
  return v.visit(*this);
}

atl::shared_ptr<ASTNode>
TypeDefDecl::accept(ASTVisitor<atl::shared_ptr<ASTNode>> &v) {
  return v.visit(*this);
}

atl::string TypeDefDecl::astClass() const { return "TypeDefDecl"; }