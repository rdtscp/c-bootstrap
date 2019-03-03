#include "../../include/ast/ConstructorDef.h"

using namespace ACC;

ConstructorDef::ConstructorDef(
    const atl::shared_ptr<Type> &classType,
    const atl::vector<atl::shared_ptr<VarDecl>> &constructorParams,
    const atl::shared_ptr<Block> &constructorBlock)
    : ConstructorDecl(classType, constructorParams),
      constructorBlock(constructorBlock) {}

atl::shared_ptr<ConstructorDecl> ConstructorDef::getptr() {
  return ConstructorDecl::shared_from_this();
}

bool ConstructorDef::operator==(Decl &rhs) const {
  if (rhs.astClass() == astClass())
    return *this == *static_cast<ConstructorDef *>(&rhs);
  return false;
}

bool ConstructorDef::operator!=(Decl &rhs) const { return !(*this == rhs); }

bool ConstructorDef::operator==(const ConstructorDef &rhs) const {
  if (constructorParams.size() != rhs.constructorParams.size())
    return false;

  for (int i = 0; i < constructorParams.size(); i++)
    if (*constructorParams[i] != *rhs.constructorParams[i])
      return false;

  /* Check Block */
  if (*constructorBlock != *rhs.constructorBlock)
    return false;

  return true;
}

bool ConstructorDef::operator!=(const ConstructorDef &rhs) const {
  return !(*this == rhs);
}

atl::string ConstructorDef::getIdentifier() const {
  return classType->identifier;
}

void ConstructorDef::accept(ASTVisitor<void> &v) { return v.visit(*this); }

atl::string ConstructorDef::accept(ASTVisitor<atl::string> &v) {
  return v.visit(*this);
}

atl::shared_ptr<Type>
ConstructorDef::accept(ASTVisitor<atl::shared_ptr<Type>> &v) {
  return v.visit(*this);
}

MIPS::Register ConstructorDef::accept(ASTVisitor<MIPS::Register> &v) {
  return v.visit(*this);
}

atl::shared_ptr<X86::Operand>
ConstructorDef::accept(ASTVisitor<atl::shared_ptr<X86::Operand>> &v) {
  return v.visit(*this);
}

atl::shared_ptr<ASTNode>
ConstructorDef::accept(ASTVisitor<atl::shared_ptr<ASTNode>> &v) {
  return v.visit(*this);
}

atl::string ConstructorDef::astClass() const { return "ConstructorDef"; }