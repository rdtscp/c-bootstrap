#include "../../include/ast/DestructorDecl.h"

using namespace ACC;

DestructorDecl::DestructorDecl(const atl::shared_ptr<Type> &classType)
    : classType(classType) {}

atl::shared_ptr<DestructorDecl> DestructorDecl::getptr() {
  return shared_from_this();
}

bool DestructorDecl::operator==(Decl &rhs) const {
  if (rhs.astClass() == astClass())
    return *this == *static_cast<FunDecl *>(&rhs);
  return false;
}

bool DestructorDecl::operator!=(Decl &rhs) const { return !(*this == rhs); }

bool DestructorDecl::operator==(const DestructorDecl &rhs) const {
  if (*classType != *rhs.classType)
    return false;

  return true;
}

bool DestructorDecl::operator!=(const DestructorDecl &rhs) const {
  return !(*this == rhs);
}

atl::shared_ptr<Identifier> DestructorDecl::getIdentifier() const {
  return classType->identifier;
}

void DestructorDecl::accept(ASTVisitor<void> &v) { return v.visit(*this); }

atl::string DestructorDecl::accept(ASTVisitor<atl::string> &v) {
  return v.visit(*this);
}

atl::shared_ptr<Type>
DestructorDecl::accept(ASTVisitor<atl::shared_ptr<Type>> &v) {
  return v.visit(*this);
}

MIPS::Register DestructorDecl::accept(ASTVisitor<MIPS::Register> &v) {
  return v.visit(*this);
}

atl::shared_ptr<X86::Operand>
DestructorDecl::accept(ASTVisitor<atl::shared_ptr<X86::Operand>> &v) {
  return v.visit(*this);
}

atl::shared_ptr<ASTNode>
DestructorDecl::accept(ASTVisitor<atl::shared_ptr<ASTNode>> &v) {
  return v.visit(*this);
}

atl::string DestructorDecl::astClass() const { return "DestructorDecl"; }