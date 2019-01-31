#include "../../include/ast/ArrayAccess.h"

using namespace ACC;

ArrayAccess::ArrayAccess(std::shared_ptr<Expr> array,
                         std::shared_ptr<Expr> index)
    : array(array), index(index) {}

std::shared_ptr<ArrayAccess> ArrayAccess::getptr() {
  return shared_from_this();
}

void ArrayAccess::accept(ASTVisitor<void> &v) { return v.visit(*this); }

atl::string ArrayAccess::accept(ASTVisitor<atl::string> &v) {
  return v.visit(*this);
}

std::shared_ptr<Type>
ArrayAccess::accept(ASTVisitor<std::shared_ptr<Type>> &v) {
  return v.visit(*this);
}

MIPS::Register ArrayAccess::accept(ASTVisitor<MIPS::Register> &v) {
  return v.visit(*this);
}

std::shared_ptr<X86::Operand>
ArrayAccess::accept(ASTVisitor<std::shared_ptr<X86::Operand>> &v) {
  return v.visit(*this);
}

std::shared_ptr<ASTNode>
ArrayAccess::accept(ASTVisitor<std::shared_ptr<ASTNode>> &v) {
  return v.visit(*this);
}

atl::string ArrayAccess::astClass() const { return "ArrayAccess"; }