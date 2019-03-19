#include "../../include/ast/Deletion.h"

using namespace ACC;

Deletion::Deletion(const DelType p_deletionType,
                   const atl::shared_ptr<VarExpr> &p_deletionVar)
    : deletionType(p_deletionType), deletionVar(p_deletionVar) {}

void Deletion::accept(ASTVisitor<void> &v) { return v.visit(*this); }

atl::shared_ptr<Deletion> Deletion::getptr() { return shared_from_this(); }

bool Deletion::operator==(const Deletion &rhs) const {
  if (deletionType != rhs.deletionType)
    return false;

  if (*deletionVar != *rhs.deletionVar)
    return false;

  return true;
};

bool Deletion::operator!=(const Deletion &rhs) const {
  return !(*this == rhs);
};

atl::string Deletion::accept(ASTVisitor<atl::string> &v) {
  return v.visit(*this);
}

atl::shared_ptr<Type> Deletion::accept(ASTVisitor<atl::shared_ptr<Type>> &v) {
  return v.visit(*this);
}

MIPS::Register Deletion::accept(ASTVisitor<MIPS::Register> &v) {
  return v.visit(*this);
}

atl::shared_ptr<X86::Operand>
Deletion::accept(ASTVisitor<atl::shared_ptr<X86::Operand>> &v) {
  return v.visit(*this);
}

atl::shared_ptr<ASTNode>
Deletion::accept(ASTVisitor<atl::shared_ptr<ASTNode>> &v) {
  return v.visit(*this);
}

atl::string Deletion::astClass() const { return "Program"; }