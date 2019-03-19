#include "../../include/ast/SizeOf.h"

using namespace ACC;

SizeOf::SizeOf(const atl::shared_ptr<Type> &type) : type(type) {}

atl::shared_ptr<SizeOf> SizeOf::getptr() { return shared_from_this(); }

bool SizeOf::operator==(Expr &rhs) const {
  if (rhs.astClass() == astClass())
    return *this == *static_cast<SizeOf *>(&rhs);
  return false;
}

bool SizeOf::operator!=(Expr &rhs) const { return !(*this == rhs); }

bool SizeOf::operator==(const SizeOf &rhs) const {
  if (*type != *rhs.type)
    return false;

  return true;
}

bool SizeOf::operator!=(const SizeOf &rhs) const { return !(*this == rhs); }

void SizeOf::accept(ASTVisitor<void> &v) { return v.visit(*this); }

atl::string SizeOf::accept(ASTVisitor<atl::string> &v) {
  return v.visit(*this);
}

atl::shared_ptr<Type> SizeOf::accept(ASTVisitor<atl::shared_ptr<Type>> &v) {
  return v.visit(*this);
}

MIPS::Register SizeOf::accept(ASTVisitor<MIPS::Register> &v) {
  return v.visit(*this);
}

atl::shared_ptr<X86::Operand>
SizeOf::accept(ASTVisitor<atl::shared_ptr<X86::Operand>> &v) {
  return v.visit(*this);
}

atl::shared_ptr<ASTNode>
SizeOf::accept(ASTVisitor<atl::shared_ptr<ASTNode>> &v) {
  return v.visit(*this);
}

atl::string SizeOf::astClass() const { return "SizeOf"; }