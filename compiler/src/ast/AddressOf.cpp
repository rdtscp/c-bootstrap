#include "../../include/ast/AddressOf.h"

using namespace ACC;

AddressOf::AddressOf(const atl::shared_ptr<Expr> &addressOfExpr)
    : addressOfExpr(addressOfExpr) {}

atl::shared_ptr<AddressOf> AddressOf::getptr() { return shared_from_this(); }

bool AddressOf::operator==(Expr &rhs) const {
  if (rhs.astClass() == astClass())
    return *this == *static_cast<AddressOf *>(&rhs);
  return false;
}

bool AddressOf::operator!=(Expr &rhs) const { return !(*this == rhs); }

bool AddressOf::operator==(const AddressOf &rhs) const {
  if (*addressOfExpr != *rhs.addressOfExpr)
    return false;

  return true;
}

bool AddressOf::operator!=(const AddressOf &rhs) const {
  return !(*this == rhs);
}

void AddressOf::accept(ASTVisitor<void> &v) { return v.visit(*this); }

atl::string AddressOf::accept(ASTVisitor<atl::string> &v) {
  return v.visit(*this);
}

atl::shared_ptr<Type> AddressOf::accept(ASTVisitor<atl::shared_ptr<Type>> &v) {
  return v.visit(*this);
}

MIPS::Register AddressOf::accept(ASTVisitor<MIPS::Register> &v) {
  return v.visit(*this);
}

atl::shared_ptr<X86::Operand>
AddressOf::accept(ASTVisitor<atl::shared_ptr<X86::Operand>> &v) {
  return v.visit(*this);
}

atl::shared_ptr<ASTNode>
AddressOf::accept(ASTVisitor<atl::shared_ptr<ASTNode>> &v) {
  return v.visit(*this);
}

atl::string AddressOf::astClass() const { return "AddressOf"; }