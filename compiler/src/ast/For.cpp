#include "../../include/ast/For.h"

using namespace ACC;

For::For() {}

atl::shared_ptr<For> For::getptr() { return shared_from_this(); }

void For::accept(ASTVisitor<void> &v) { return v.visit(*this); }

atl::string For::accept(ASTVisitor<atl::string> &v) { return v.visit(*this); }

atl::shared_ptr<Type> For::accept(ASTVisitor<atl::shared_ptr<Type>> &v) {
  return v.visit(*this);
}

MIPS::Register For::accept(ASTVisitor<MIPS::Register> &v) {
  return v.visit(*this);
}

atl::shared_ptr<X86::Operand>
For::accept(ASTVisitor<atl::shared_ptr<X86::Operand>> &v) {
  return v.visit(*this);
}

atl::shared_ptr<ASTNode> For::accept(ASTVisitor<atl::shared_ptr<ASTNode>> &v) {
  return v.visit(*this);
}

atl::string For::astClass() const { return "For"; }