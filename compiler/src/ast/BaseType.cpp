#include "../../include/ast/BaseType.h"

using namespace ACC;

BaseType::BaseType(PrimitiveType pType) : primitiveType(pType) {}

atl::shared_ptr<BaseType> BaseType::getptr() { return shared_from_this(); }

bool BaseType::operator==(Type &rhs) const {
  if (rhs.astClass() == astClass())
    return *this == *static_cast<BaseType *>(&rhs);
  return false;
}
bool BaseType::operator!=(Type &t) const { return !(*this == t); }

bool BaseType::operator==(const BaseType &rhs) const {
  return primitiveType == rhs.primitiveType;
}
bool BaseType::operator!=(const BaseType &rhs) const { return !(*this == rhs); }

int BaseType::getBytes() const {
  switch (primitiveType) {
  case PrimitiveType::CHAR:
    return 1;
  case PrimitiveType::INT:
    return 4;
  case PrimitiveType::SHORT:
    return 4;
  case PrimitiveType::VOID:
    return 4;
  case PrimitiveType::UINT:
    return 4;
  }
}

void BaseType::accept(ASTVisitor<void> &v) { return v.visit(*this); }

atl::string BaseType::accept(ASTVisitor<atl::string> &v) {
  return v.visit(*this);
}

atl::shared_ptr<Type> BaseType::accept(ASTVisitor<atl::shared_ptr<Type>> &v) {
  return v.visit(*this);
}

MIPS::Register BaseType::accept(ASTVisitor<MIPS::Register> &v) {
  return v.visit(*this);
}

atl::shared_ptr<X86::Operand>
BaseType::accept(ASTVisitor<atl::shared_ptr<X86::Operand>> &v) {
  return v.visit(*this);
}

atl::shared_ptr<ASTNode>
BaseType::accept(ASTVisitor<atl::shared_ptr<ASTNode>> &v) {
  return v.visit(*this);
}

atl::string BaseType::astClass() const { return "BaseType"; }