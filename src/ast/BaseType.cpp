#include "../../include/ast/BaseType.h"

using namespace ACC;

BaseType::BaseType(PrimitiveType pType) : primitiveType(pType) {}

bool BaseType::operator==(Type &t) const {
  if (t.astClass() == astClass())
    return *this == *static_cast<BaseType *>(&t);
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
  case PrimitiveType::VOID:
    return 4;
  }
}

void BaseType::accept(ASTVisitor<void> &v) { return v.visit(*this); }

std::string BaseType::accept(ASTVisitor<std::string> &v) {
  return v.visit(*this);
}

std::shared_ptr<Type> BaseType::accept(ASTVisitor<std::shared_ptr<Type>> &v) {
  return v.visit(*this);
}

MIPS::Register BaseType::accept(ASTVisitor<MIPS::Register> &v) {
  return v.visit(*this);
}

std::shared_ptr<X86::Operand> BaseType::accept(ASTVisitor<std::shared_ptr<X86::Operand>> &v) {
  return v.visit(*this);
}

std::string BaseType::astClass() const { return "BaseType"; }