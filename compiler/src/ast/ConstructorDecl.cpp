#include "../../include/ast/ConstructorDecl.h"

using namespace ACC;

ConstructorDecl::ConstructorDecl(
    const atl::shared_ptr<Type> &classType,
    const atl::vector<atl::shared_ptr<VarDecl>> &constructorParams)
    : classType(classType), constructorParams(constructorParams) {}

atl::shared_ptr<ConstructorDecl> ConstructorDecl::getptr() {
  return shared_from_this();
}

bool ConstructorDecl::operator==(Decl &rhs) const {
  if (rhs.astClass() == astClass())
    return *this == *static_cast<FunDecl *>(&rhs);
  return false;
}

bool ConstructorDecl::operator!=(Decl &rhs) const { return !(*this == rhs); }

bool ConstructorDecl::operator==(const ConstructorDecl &rhs) const {
  if (*classType != *rhs.classType)
    return false;

  if (constructorParams.size() != rhs.constructorParams.size())
    return false;

  for (int i = 0; i < constructorParams.size(); ++i)
    if (*constructorParams[i] != *rhs.constructorParams[i])
      return false;

  return true;
}

bool ConstructorDecl::operator!=(const ConstructorDecl &rhs) const {
  return !(*this == rhs);
}

atl::string ConstructorDecl::getIdentifier() const {
  return classType->identifier;
}

void ConstructorDecl::accept(ASTVisitor<void> &v) { return v.visit(*this); }

atl::string ConstructorDecl::accept(ASTVisitor<atl::string> &v) {
  return v.visit(*this);
}

atl::shared_ptr<Type>
ConstructorDecl::accept(ASTVisitor<atl::shared_ptr<Type>> &v) {
  return v.visit(*this);
}

MIPS::Register ConstructorDecl::accept(ASTVisitor<MIPS::Register> &v) {
  return v.visit(*this);
}

atl::shared_ptr<X86::Operand>
ConstructorDecl::accept(ASTVisitor<atl::shared_ptr<X86::Operand>> &v) {
  return v.visit(*this);
}

atl::shared_ptr<ASTNode>
ConstructorDecl::accept(ASTVisitor<atl::shared_ptr<ASTNode>> &v) {
  return v.visit(*this);
}

atl::string ConstructorDecl::astClass() const { return "ConstructorDecl"; }