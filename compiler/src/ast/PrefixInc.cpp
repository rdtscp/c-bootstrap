#include "../../include/ast/PrefixInc.h"

using namespace ACC;

PrefixInc::PrefixInc(const atl::shared_ptr<VarExpr> &incrementVar)
    : incrementVar(incrementVar) {}

atl::shared_ptr<PrefixInc> PrefixInc::getptr() { return shared_from_this(); }

bool PrefixInc::operator==(Expr &rhs) const {
  if (rhs.astClass() == astClass())
    return *this == *static_cast<PrefixInc *>(&rhs);
  return false;
}

bool PrefixInc::operator!=(Expr &rhs) const { return !(*this == rhs); }

bool PrefixInc::operator==(const PrefixInc &rhs) const {
  if (*incrementVar != *rhs.incrementVar)
    return false;

  return true;
}

bool PrefixInc::operator!=(const PrefixInc &rhs) const {
  return !(*this == rhs);
}

void PrefixInc::accept(ASTVisitor<void> &v) { return v.visit(*this); }

atl::string PrefixInc::accept(ASTVisitor<atl::string> &v) {
  return v.visit(*this);
}

atl::shared_ptr<Type> PrefixInc::accept(ASTVisitor<atl::shared_ptr<Type>> &v) {
  return v.visit(*this);
}

MIPS::Register PrefixInc::accept(ASTVisitor<MIPS::Register> &v) {
  return v.visit(*this);
}

atl::shared_ptr<X86::Operand>
PrefixInc::accept(ASTVisitor<atl::shared_ptr<X86::Operand>> &v) {
  return v.visit(*this);
}

atl::shared_ptr<ASTNode>
PrefixInc::accept(ASTVisitor<atl::shared_ptr<ASTNode>> &v) {
  return v.visit(*this);
}

atl::string PrefixInc::astClass() const { return "PrefixInc"; }