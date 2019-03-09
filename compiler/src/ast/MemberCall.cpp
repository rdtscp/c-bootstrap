#include "../../include/ast/MemberCall.h"

using namespace ACC;

MemberCall::MemberCall(atl::shared_ptr<Expr> object,
                       const atl::shared_ptr<FunCall> funCall)
    : object(object), funCall(funCall) {}

atl::shared_ptr<MemberCall> MemberCall::getptr() { return shared_from_this(); }

bool MemberCall::operator==(Expr &rhs) const {
  if (rhs.astClass() == astClass())
    return *this == *static_cast<MemberCall *>(&rhs);
  return false;
}

bool MemberCall::operator!=(Expr &rhs) const { return !(*this == rhs); }

bool MemberCall::operator==(const MemberCall &rhs) const {
  if (*object != *rhs.object)
    return false;

  if (*funCall != *rhs.funCall)
    return false;

  return true;
}

bool MemberCall::operator!=(const MemberCall &rhs) const {
  return !(*this == rhs);
}

void MemberCall::accept(ASTVisitor<void> &v) { return v.visit(*this); }

atl::string MemberCall::accept(ASTVisitor<atl::string> &v) {
  return v.visit(*this);
}

atl::shared_ptr<Type> MemberCall::accept(ASTVisitor<atl::shared_ptr<Type>> &v) {
  return v.visit(*this);
}

MIPS::Register MemberCall::accept(ASTVisitor<MIPS::Register> &v) {
  return v.visit(*this);
}

atl::shared_ptr<X86::Operand>
MemberCall::accept(ASTVisitor<atl::shared_ptr<X86::Operand>> &v) {
  return v.visit(*this);
}

atl::shared_ptr<ASTNode>
MemberCall::accept(ASTVisitor<atl::shared_ptr<ASTNode>> &v) {
  return v.visit(*this);
}

atl::string MemberCall::astClass() const { return "MemberCall"; }