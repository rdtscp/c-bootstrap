#include "../../include/ast/ClassTypeDecl.h"

using namespace ACC;

ClassTypeDecl::ClassTypeDecl(
    atl::shared_ptr<ClassType> classType,
    const atl::vector<atl::shared_ptr<Decl>> &classDecls)
    : classType(classType), classDecls(classDecls) {}

atl::shared_ptr<ClassTypeDecl> ClassTypeDecl::getptr() {
  return shared_from_this();
}

bool ClassTypeDecl::operator==(Decl &rhs) const {
  if (rhs.astClass() == astClass())
    return *this == *static_cast<ClassTypeDecl *>(&rhs);
  return false;
}

bool ClassTypeDecl::operator!=(Decl &rhs) const { return !(*this == rhs); }

bool ClassTypeDecl::operator==(const ClassTypeDecl &rhs) const {
  if (*classType != *rhs.classType)
    return false;

  if (classDecls.size() != rhs.classDecls.size())
    return false;

  for (int i = 0; i < classDecls.size(); i++)
    if (*classDecls[i] != *rhs.classDecls[i])
      return false;

  return true;
}

bool ClassTypeDecl::operator!=(const ClassTypeDecl &rhs) const {
  return !(*this == rhs);
}

atl::string ClassTypeDecl::getIdentifier() const {
  return atl::string("class ") + classType->identifier;
}

void ClassTypeDecl::accept(ASTVisitor<void> &v) { return v.visit(*this); }

atl::string ClassTypeDecl::accept(ASTVisitor<atl::string> &v) {
  return v.visit(*this);
}

atl::shared_ptr<Type>
ClassTypeDecl::accept(ASTVisitor<atl::shared_ptr<Type>> &v) {
  return v.visit(*this);
}

MIPS::Register ClassTypeDecl::accept(ASTVisitor<MIPS::Register> &v) {
  return v.visit(*this);
}

atl::shared_ptr<X86::Operand>
ClassTypeDecl::accept(ASTVisitor<atl::shared_ptr<X86::Operand>> &v) {
  return v.visit(*this);
}

atl::shared_ptr<ASTNode>
ClassTypeDecl::accept(ASTVisitor<atl::shared_ptr<ASTNode>> &v) {
  return v.visit(*this);
}

atl::string ClassTypeDecl::astClass() const { return "ClassTypeDecl"; }