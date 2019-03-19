#include "../../include/ast/FunDecl.h"

using namespace ACC;

FunDecl::FunDecl(const atl::shared_ptr<Identifier> &funIdentifier,
                 const atl::vector<atl::shared_ptr<VarDecl>> &funParams,
                 const atl::shared_ptr<Type> &funType)
    : funIdentifier(funIdentifier), funParams(funParams), funType(funType) {}

atl::shared_ptr<FunDecl> FunDecl::getptr() { return shared_from_this(); }

bool FunDecl::operator==(Decl &rhs) const {
  if (rhs.astClass() == astClass())
    return *this == *static_cast<FunDecl *>(&rhs);
  return false;
}

bool FunDecl::operator!=(Decl &rhs) const { return !(*this == rhs); }

bool FunDecl::operator==(const FunDecl &rhs) const {
  if (*funType != *rhs.funType)
    return false;

  if (*funIdentifier != *rhs.funIdentifier)
    return false;

  if (funParams.size() != rhs.funParams.size())
    return false;

  for (int i = 0; i < funParams.size(); ++i)
    if (*funParams[i] != *rhs.funParams[i])
      return false;

  return true;
}

bool FunDecl::operator!=(const FunDecl &rhs) const { return !(*this == rhs); }

atl::shared_ptr<Identifier> FunDecl::getIdentifier() const {
  return funIdentifier;
}

void FunDecl::accept(ASTVisitor<void> &v) { return v.visit(*this); }

atl::string FunDecl::accept(ASTVisitor<atl::string> &v) {
  return v.visit(*this);
}

atl::shared_ptr<Type> FunDecl::accept(ASTVisitor<atl::shared_ptr<Type>> &v) {
  return v.visit(*this);
}

MIPS::Register FunDecl::accept(ASTVisitor<MIPS::Register> &v) {
  return v.visit(*this);
}

atl::shared_ptr<X86::Operand>
FunDecl::accept(ASTVisitor<atl::shared_ptr<X86::Operand>> &v) {
  return v.visit(*this);
}

atl::shared_ptr<ASTNode>
FunDecl::accept(ASTVisitor<atl::shared_ptr<ASTNode>> &v) {
  return v.visit(*this);
}

atl::string FunDecl::astClass() const { return "FunDecl"; }