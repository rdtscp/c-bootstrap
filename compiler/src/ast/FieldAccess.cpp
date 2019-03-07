#include "../../include/ast/FieldAccess.h"

using namespace ACC;

FieldAccess::FieldAccess(atl::shared_ptr<Expr> object, const atl::string &field)
    : object(object), field(field) {}

atl::shared_ptr<FieldAccess> FieldAccess::getptr() {
  return shared_from_this();
}

bool FieldAccess::operator==(Expr &rhs) const {
  if (rhs.astClass() == astClass())
    return *this == *static_cast<FieldAccess *>(&rhs);
  return false;
}

bool FieldAccess::operator!=(Expr &rhs) const { return !(*this == rhs); }

bool FieldAccess::operator==(const FieldAccess &rhs) const {
  if (*object != *rhs.object)
    return false;

  if (field != rhs.field)
    return false;

  return true;
}

bool FieldAccess::operator!=(const FieldAccess &rhs) const {
  return !(*this == rhs);
}

void FieldAccess::accept(ASTVisitor<void> &v) { return v.visit(*this); }

atl::string FieldAccess::accept(ASTVisitor<atl::string> &v) {
  return v.visit(*this);
}

atl::shared_ptr<Type>
FieldAccess::accept(ASTVisitor<atl::shared_ptr<Type>> &v) {
  return v.visit(*this);
}

MIPS::Register FieldAccess::accept(ASTVisitor<MIPS::Register> &v) {
  return v.visit(*this);
}

atl::shared_ptr<X86::Operand>
FieldAccess::accept(ASTVisitor<atl::shared_ptr<X86::Operand>> &v) {
  return v.visit(*this);
}

atl::shared_ptr<ASTNode>
FieldAccess::accept(ASTVisitor<atl::shared_ptr<ASTNode>> &v) {
  return v.visit(*this);
}

atl::string FieldAccess::astClass() const { return "FieldAccess"; }