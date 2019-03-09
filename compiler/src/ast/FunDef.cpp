#include "../../include/ast/FunDef.h"

using namespace ACC;

FunDef::FunDef(atl::shared_ptr<Block> funBlock, const atl::string &funName,
               atl::vector<atl::shared_ptr<VarDecl>> funParams,
               atl::shared_ptr<Type> funType)
    : FunDecl(funName, funParams, funType), funBlock(funBlock) {}

atl::shared_ptr<FunDecl> FunDef::getptr() {
  return FunDecl::shared_from_this();
}

bool FunDef::operator==(Decl &rhs) const {
  if (rhs.astClass() == astClass())
    return *this == *static_cast<FunDef *>(&rhs);
  return false;
}

bool FunDef::operator!=(Decl &rhs) const { return !(*this == rhs); }

bool FunDef::operator==(const FunDef &rhs) const {
  if (*funType != *rhs.funType)
    return false;

  if (funName != rhs.funName)
    return false;

  if (funParams.size() != rhs.funParams.size())
    return false;

  for (int i = 0; i < funParams.size(); ++i)
    if (*funParams[i] != *rhs.funParams[i])
      return false;

  /* Check Block*/
  if (*funBlock != *rhs.funBlock)
    return false;

  return true;
}

bool FunDef::operator!=(const FunDef &rhs) const { return !(*this == rhs); }

atl::string FunDef::getIdentifier() const { return funName; }

void FunDef::accept(ASTVisitor<void> &v) { return v.visit(*this); }

atl::string FunDef::accept(ASTVisitor<atl::string> &v) {
  return v.visit(*this);
}

atl::shared_ptr<Type> FunDef::accept(ASTVisitor<atl::shared_ptr<Type>> &v) {
  return v.visit(*this);
}

MIPS::Register FunDef::accept(ASTVisitor<MIPS::Register> &v) {
  return v.visit(*this);
}

atl::shared_ptr<X86::Operand>
FunDef::accept(ASTVisitor<atl::shared_ptr<X86::Operand>> &v) {
  return v.visit(*this);
}

atl::shared_ptr<ASTNode>
FunDef::accept(ASTVisitor<atl::shared_ptr<ASTNode>> &v) {
  return v.visit(*this);
}

atl::string FunDef::astClass() const { return "FunDef"; }