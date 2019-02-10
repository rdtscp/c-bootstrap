#include "../../include/ast/PointerType.h"

using namespace ACC;

PointerType::PointerType(atl::shared_ptr<Type> pointedType)
    : pointedType(pointedType) {}

atl::shared_ptr<PointerType> PointerType::getptr() {
  return shared_from_this();
}

bool PointerType::operator==(Type &rhs) const {
  if (rhs.astClass() == astClass())
    return *this == *static_cast<PointerType *>(&rhs);
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

atl::string PointerType::accept(ASTVisitor<atl::string> &v) {
  return v.visit(*this);
}

atl::shared_ptr<Type>
PointerType::accept(ASTVisitor<atl::shared_ptr<Type>> &v) {
  return v.visit(*this);
}

MIPS::Register PointerType::accept(ASTVisitor<MIPS::Register> &v) {
  return v.visit(*this);
}

atl::shared_ptr<X86::Operand>
PointerType::accept(ASTVisitor<atl::shared_ptr<X86::Operand>> &v) {
  return v.visit(*this);
}

atl::shared_ptr<ASTNode>
PointerType::accept(ASTVisitor<atl::shared_ptr<ASTNode>> &v) {
  return v.visit(*this);
}

atl::string PointerType::astClass() const { return "PointerType"; }