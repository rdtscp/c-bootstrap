#include "../../include/ast/ClassType.h"

using namespace ACC;

ClassType::ClassType(const atl::string &identifier) : identifier(identifier) {}

atl::shared_ptr<ClassType> ClassType::getptr() { return shared_from_this(); }

int ClassType::getBytes() const {
  int aggregateBytes = 0;
  if (typeDefinition == nullptr)
    return aggregateBytes;

  // @TODO: Calculate SUM of VarDecls.
  const int classDeclsSize = typeDefinition->classDecls.size();
  for (int i = 0; i < classDeclsSize; ++i) {
    if (typeDefinition->classDecls[i]->astClass() == "VarDecl") {
      aggregateBytes +=
          atl::static_pointer_cast<VarDecl>(typeDefinition->classDecls[i])
              ->getBytes();
    }
  }
  // atl::vector<atl::shared_ptr<VarDecl>> typeVarDecls =
  // typeDefinition->varDecls; for (int idx = 0; idx < typeVarDecls.size();
  // ++idx) {
  //   const atl::shared_ptr<VarDecl> &structField = typeVarDecls[idx];
  //   aggregateBytes += structField->type->getBytes();
  // }

  return aggregateBytes;
}

bool ClassType::operator==(Type &rhs) const {
  if (rhs.astClass() == astClass())
    return *this == *static_cast<ClassType *>(&rhs);
  return false;
}

bool ClassType::operator!=(Type &rhs) const { return !(*this == rhs); }

bool ClassType::operator==(const ClassType &rhs) const {
  return identifier == rhs.identifier;
}

bool ClassType::operator!=(const ClassType &rhs) const {
  return !(*this == rhs);
}

void ClassType::accept(ASTVisitor<void> &v) { return v.visit(*this); }

atl::string ClassType::accept(ASTVisitor<atl::string> &v) {
  return v.visit(*this);
}

atl::shared_ptr<Type> ClassType::accept(ASTVisitor<atl::shared_ptr<Type>> &v) {
  return v.visit(*this);
}

MIPS::Register ClassType::accept(ASTVisitor<MIPS::Register> &v) {
  return v.visit(*this);
}

atl::shared_ptr<X86::Operand>
ClassType::accept(ASTVisitor<atl::shared_ptr<X86::Operand>> &v) {
  return v.visit(*this);
}

atl::shared_ptr<ASTNode>
ClassType::accept(ASTVisitor<atl::shared_ptr<ASTNode>> &v) {
  return v.visit(*this);
}

atl::string ClassType::astClass() const { return "ClassType"; }