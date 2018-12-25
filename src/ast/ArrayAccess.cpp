#include "../../include/ast/ArrayAccess.h"

using namespace ACC;

ArrayAccess::ArrayAccess(std::shared_ptr<Expr> array,
                         std::shared_ptr<Expr> index)
    : array(array), index(index) {}

void ArrayAccess::accept(ASTVisitor<void> &v) { return v.visit(*this); }
std::string ArrayAccess::accept(ASTVisitor<std::string> &v) {
  return v.visit(*this);
}
std::shared_ptr<Type>
ArrayAccess::accept(ASTVisitor<std::shared_ptr<Type>> &v) {
  return v.visit(*this);
}
MIPS::Register ArrayAccess::accept(ASTVisitor<MIPS::Register> &v) {
  return v.visit(*this);
}
X86::Register ArrayAccess::accept(ASTVisitor<X86::Register> &v) {
  return v.visit(*this);
}
std::string ArrayAccess::astClass() const { return "ArrayAccess"; }