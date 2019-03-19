#include "../../include/ast/ArrayAccess.h"

using namespace ACC;

ArrayAccess::ArrayAccess(const atl::shared_ptr<Expr> &p_array,
                         const atl::shared_ptr<Expr> &p_index)
    : array(p_array), index(p_index) {}

atl::shared_ptr<ArrayAccess> ArrayAccess::getptr() {
  return shared_from_this();
}

bool ArrayAccess::operator==(Expr &rhs) const {
  if (rhs.astClass() == astClass())
    return *this == *static_cast<ArrayAccess *>(&rhs);
  return false;
}

bool ArrayAccess::operator!=(Expr &rhs) const { return !(*this == rhs); }

bool ArrayAccess::operator==(const ArrayAccess &rhs) const {
  if (*array != *rhs.array)
    return false;

  if (index != rhs.index)
    return false;

  return true;
}

bool ArrayAccess::operator!=(const ArrayAccess &rhs) const {
  return !(*this == rhs);
}
void ArrayAccess::accept(ASTVisitor<void> &v) { return v.visit(*this); }

atl::string ArrayAccess::accept(ASTVisitor<atl::string> &v) {
  return v.visit(*this);
}

atl::shared_ptr<Type>
ArrayAccess::accept(ASTVisitor<atl::shared_ptr<Type>> &v) {
  return v.visit(*this);
}

MIPS::Register ArrayAccess::accept(ASTVisitor<MIPS::Register> &v) {
  return v.visit(*this);
}

atl::shared_ptr<X86::Operand>
ArrayAccess::accept(ASTVisitor<atl::shared_ptr<X86::Operand>> &v) {
  return v.visit(*this);
}

atl::shared_ptr<ASTNode>
ArrayAccess::accept(ASTVisitor<atl::shared_ptr<ASTNode>> &v) {
  return v.visit(*this);
}

atl::string ArrayAccess::astClass() const { return "ArrayAccess"; }