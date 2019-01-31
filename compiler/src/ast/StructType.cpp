#include "../../include/ast/StructType.h"

using namespace ACC;

StructType::StructType(const atl::string &identifier)
    : identifier(identifier) {}

std::shared_ptr<StructType> StructType::getptr() { return shared_from_this(); }

int StructType::getBytes() const {
  int aggregateBytes = 0;
  if (typeDefinition == nullptr)
    return aggregateBytes;

  for (const std::shared_ptr<VarDecl> &structField : typeDefinition->varDecls)
    aggregateBytes += structField->type->getBytes();

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

std::shared_ptr<Type> StructType::accept(ASTVisitor<std::shared_ptr<Type>> &v) {
  return v.visit(*this);
}

MIPS::Register StructType::accept(ASTVisitor<MIPS::Register> &v) {
  return v.visit(*this);
}

std::shared_ptr<X86::Operand>
StructType::accept(ASTVisitor<std::shared_ptr<X86::Operand>> &v) {
  return v.visit(*this);
}

std::shared_ptr<ASTNode>
StructType::accept(ASTVisitor<std::shared_ptr<ASTNode>> &v) {
  return v.visit(*this);
}

atl::string StructType::astClass() const { return "StructType"; }