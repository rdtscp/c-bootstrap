#include "../../include/ast/PointerType.h"

using namespace ACC;

PointerType::PointerType(std::shared_ptr<Type> pointedType)
    : pointedType(pointedType) {}

bool PointerType::operator==(Type &t) const {
  if (t.astClass() == astClass())
    return *this == *static_cast<PointerType *>(&t);
  return false;
}

bool PointerType::operator!=(Type &t) const { return !(*this == t); }

bool PointerType::operator==(const PointerType &rhs) const {
  return *pointedType == *rhs.pointedType;
}

bool PointerType::operator!=(const PointerType &rhs) const {
  return !(*this == rhs);
}

int PointerType::getBytes() const { return 4; }

void PointerType::accept(ASTVisitor<void> &v) { return v.visit(*this); }

std::string PointerType::accept(ASTVisitor<std::string> &v) {
  return v.visit(*this);
}

std::shared_ptr<Type>
PointerType::accept(ASTVisitor<std::shared_ptr<Type>> &v) {
  return v.visit(*this);
}

MIPS::Register PointerType::accept(ASTVisitor<MIPS::Register> &v) {
  return v.visit(*this);
}

std::shared_ptr<X86::Operand> PointerType::accept(ASTVisitor<std::shared_ptr<X86::Operand>> &v) {
  return v.visit(*this);
}

std::string PointerType::astClass() const { return "PointerType"; }