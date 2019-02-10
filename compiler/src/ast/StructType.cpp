#include "../../include/ast/StructType.h"

using namespace ACC;

StructType::StructType(const atl::string &identifier)
    : identifier(identifier) {}

atl::shared_ptr<StructType> StructType::getptr() { return shared_from_this(); }

int StructType::getBytes() const {
  int aggregateBytes = 0;
  if (typeDefinition == nullptr)
    return aggregateBytes;

  atl::vector<atl::shared_ptr<VarDecl>> typeVarDecls = typeDefinition->varDecls;
  for (int idx = 0; idx < typeVarDecls.size(); ++idx) {
    const atl::shared_ptr<VarDecl> &structField = typeVarDecls[idx];
    aggregateBytes += structField->type->getBytes();
  }

  return aggregateBytes;
}

bool StructType::operator==(Type &rhs) const {
  if (rhs.astClass() == astClass())
    return *this == *static_cast<StructType *>(&rhs);
  return false;
}

bool StructType::operator!=(Type &rhs) const { return !(*this == rhs); }

bool StructType::operator==(const StructType &rhs) const {
  return identifier == rhs.identifier;
}

bool StructType::operator!=(const StructType &rhs) const {
  return !(*this == rhs);
}

void StructType::accept(ASTVisitor<void> &v) { return v.visit(*this); }

atl::string StructType::accept(ASTVisitor<atl::string> &v) {
  return v.visit(*this);
}

atl::shared_ptr<Type> StructType::accept(ASTVisitor<atl::shared_ptr<Type>> &v) {
  return v.visit(*this);
}

MIPS::Register StructType::accept(ASTVisitor<MIPS::Register> &v) {
  return v.visit(*this);
}

atl::shared_ptr<X86::Operand>
StructType::accept(ASTVisitor<atl::shared_ptr<X86::Operand>> &v) {
  return v.visit(*this);
}

atl::shared_ptr<ASTNode>
StructType::accept(ASTVisitor<atl::shared_ptr<ASTNode>> &v) {
  return v.visit(*this);
}

atl::string StructType::astClass() const { return "StructType"; }