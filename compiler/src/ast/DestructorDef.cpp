#include "../../include/ast/DestructorDef.h"

using namespace ACC;

DestructorDef::DestructorDef(const atl::shared_ptr<Type> &p_classType,
                             const atl::shared_ptr<Block> &p_destructorBlock)
    : DestructorDecl(p_classType), destructorBlock(p_destructorBlock) {}

atl::shared_ptr<DestructorDecl> DestructorDef::getptr() {
  return DestructorDecl::shared_from_this();
}

bool DestructorDef::operator==(Decl &rhs) const {
  if (rhs.astClass() == astClass())
    return *this == *static_cast<DestructorDef *>(&rhs);
  return false;
}

bool DestructorDef::operator!=(Decl &rhs) const { return !(*this == rhs); }

bool DestructorDef::operator==(const DestructorDef &rhs) const {
  if (*classType != *rhs.classType)
    return false;

  /* Check Block */
  if (*destructorBlock != *rhs.destructorBlock)
    return false;

  return true;
}

bool DestructorDef::operator!=(const DestructorDef &rhs) const {
  return !(*this == rhs);
}

atl::shared_ptr<Identifier> DestructorDef::getIdentifier() const {
  return classType->identifier;
}

void DestructorDef::accept(ASTVisitor<void> &v) { return v.visit(*this); }

atl::string DestructorDef::accept(ASTVisitor<atl::string> &v) {
  return v.visit(*this);
}

atl::shared_ptr<Type>
DestructorDef::accept(ASTVisitor<atl::shared_ptr<Type>> &v) {
  return v.visit(*this);
}

MIPS::Register DestructorDef::accept(ASTVisitor<MIPS::Register> &v) {
  return v.visit(*this);
}

atl::shared_ptr<X86::Operand>
DestructorDef::accept(ASTVisitor<atl::shared_ptr<X86::Operand>> &v) {
  return v.visit(*this);
}

atl::shared_ptr<ASTNode>
DestructorDef::accept(ASTVisitor<atl::shared_ptr<ASTNode>> &v) {
  return v.visit(*this);
}

atl::string DestructorDef::astClass() const { return "DestructorDef"; }