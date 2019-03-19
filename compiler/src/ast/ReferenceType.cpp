#include "../../include/ast/ReferenceType.h"

using namespace ACC;

ReferenceType::ReferenceType(const atl::shared_ptr<Type> &referencedType)
    : referencedType(referencedType) {}

atl::shared_ptr<ReferenceType> ReferenceType::getptr() {
  return shared_from_this();
}

bool ReferenceType::operator==(Type &rhs) const {
  if (rhs.astClass() == astClass())
    return *this == *static_cast<ReferenceType *>(&rhs);
  return false;
}

bool ReferenceType::operator!=(Type &t) const { return !(*this == t); }

bool ReferenceType::operator==(const ReferenceType &rhs) const {
  return *referencedType == *rhs.referencedType;
}

bool ReferenceType::operator!=(const ReferenceType &rhs) const {
  return !(*this == rhs);
}

int ReferenceType::getBytes() const { return 4; }

void ReferenceType::accept(ASTVisitor<void> &v) { return v.visit(*this); }

atl::string ReferenceType::accept(ASTVisitor<atl::string> &v) {
  return v.visit(*this);
}

atl::shared_ptr<Type>
ReferenceType::accept(ASTVisitor<atl::shared_ptr<Type>> &v) {
  return v.visit(*this);
}

MIPS::Register ReferenceType::accept(ASTVisitor<MIPS::Register> &v) {
  return v.visit(*this);
}

atl::shared_ptr<X86::Operand>
ReferenceType::accept(ASTVisitor<atl::shared_ptr<X86::Operand>> &v) {
  return v.visit(*this);
}

atl::shared_ptr<ASTNode>
ReferenceType::accept(ASTVisitor<atl::shared_ptr<ASTNode>> &v) {
  return v.visit(*this);
}

atl::string ReferenceType::astClass() const { return "ReferenceType"; }