#include "../../include/ast/MemberAccess.h"

using namespace ACC;

MemberAccess::MemberAccess(const atl::shared_ptr<Expr> &object,
                           const atl::shared_ptr<Identifier> &fieldIdentifier)
    : object(object), fieldIdentifier(fieldIdentifier) {}

atl::shared_ptr<MemberAccess> MemberAccess::getptr() {
  return shared_from_this();
}

bool MemberAccess::operator==(Expr &rhs) const {
  if (rhs.astClass() == astClass())
    return *this == *static_cast<MemberAccess *>(&rhs);
  return false;
}

bool MemberAccess::operator!=(Expr &rhs) const { return !(*this == rhs); }

bool MemberAccess::operator==(const MemberAccess &rhs) const {
  if (*object != *rhs.object)
    return false;

  if (*fieldIdentifier != *rhs.fieldIdentifier)
    return false;

  return true;
}

bool MemberAccess::operator!=(const MemberAccess &rhs) const {
  return !(*this == rhs);
}

void MemberAccess::accept(ASTVisitor<void> &v) { return v.visit(*this); }

atl::string MemberAccess::accept(ASTVisitor<atl::string> &v) {
  return v.visit(*this);
}

atl::shared_ptr<Type>
MemberAccess::accept(ASTVisitor<atl::shared_ptr<Type>> &v) {
  return v.visit(*this);
}

MIPS::Register MemberAccess::accept(ASTVisitor<MIPS::Register> &v) {
  return v.visit(*this);
}

atl::shared_ptr<X86::Operand>
MemberAccess::accept(ASTVisitor<atl::shared_ptr<X86::Operand>> &v) {
  return v.visit(*this);
}

atl::shared_ptr<ASTNode>
MemberAccess::accept(ASTVisitor<atl::shared_ptr<ASTNode>> &v) {
  return v.visit(*this);
}

atl::string MemberAccess::astClass() const { return "MemberAccess"; }