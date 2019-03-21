#include "../../include/ast/FunDecl.h"

using namespace ACC;

FunDecl::FunDecl(const atl::set<FunModifiers> &p_funModifiers,
                 const atl::shared_ptr<Identifier> &p_funIdentifier,
                 const atl::vector<atl::shared_ptr<VarDecl>> &p_funParams,
                 const atl::shared_ptr<Type> &p_funType)
    : funModifiers(p_funModifiers), funIdentifier(p_funIdentifier), funParams(p_funParams),
      funType(p_funType) {}

atl::shared_ptr<FunDecl> FunDecl::getptr() { return shared_from_this(); }

bool FunDecl::operator==(Decl &rhs) const {
  if (rhs.astClass() == astClass())
    return *this == *static_cast<FunDecl *>(&rhs);
  return false;
}

bool FunDecl::operator!=(Decl &rhs) const { return !(*this == rhs); }

bool FunDecl::operator==(const FunDecl &rhs) const {
  if (funModifiers != rhs.funModifiers)
    return false;
    
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
atl::string FunDecl::getSignature() const {
  atl::string signatureStr = funIdentifier->toString() + "(";
  for (int idx = 0; idx < funParams.size(); ++idx) {
    signatureStr += funParams[idx]->type->getSignature();
    if (idx < (funParams.size() - 1))
      signatureStr += ", ";
  }
  return signatureStr + ")";
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