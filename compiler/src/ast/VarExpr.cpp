#include "../../include/ast/VarExpr.h"

using namespace ACC;

VarExpr::VarExpr(const atl::string &identifier) : identifier(identifier) {}

std::shared_ptr<VarExpr> VarExpr::getptr() { return shared_from_this(); }

void VarExpr::accept(ASTVisitor<void> &v) { return v.visit(*this); }

atl::string VarExpr::accept(ASTVisitor<atl::string> &v) {
  return v.visit(*this);
}

std::shared_ptr<Type> VarExpr::accept(ASTVisitor<std::shared_ptr<Type>> &v) {
  return v.visit(*this);
}

MIPS::Register VarExpr::accept(ASTVisitor<MIPS::Register> &v) {
  return v.visit(*this);
}

std::shared_ptr<X86::Operand>
VarExpr::accept(ASTVisitor<std::shared_ptr<X86::Operand>> &v) {
  return v.visit(*this);
}

std::shared_ptr<ASTNode>
VarExpr::accept(ASTVisitor<std::shared_ptr<ASTNode>> &v) {
  return v.visit(*this);
}

atl::string VarExpr::astClass() const { return "VarExpr"; }