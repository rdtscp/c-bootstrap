#include "../../include/ast/FunDecl.h"

using namespace ACC;

FunDecl::FunDecl(std::shared_ptr<Block> funBlock, std::string funName,
                 std::vector<std::shared_ptr<VarDecl>> funParams,
                 std::shared_ptr<Type> funType)
    : funBlock(funBlock), funName(funName), funParams(funParams),
      funType(funType) {}

std::shared_ptr<FunDecl> FunDecl::getptr() { return shared_from_this(); }

bool FunDecl::operator==(Decl &rhs) const {
  if (rhs.astClass() == astClass())
    return *this == *static_cast<FunDecl *>(&rhs);
  return false;
}

bool FunDecl::operator!=(Decl &rhs) const { return !(*this == rhs); }

bool FunDecl::operator==(const FunDecl &rhs) const {
  if (*funType != *rhs.funType)
    return false;

  if (funName != rhs.funName)
    return false;

  if (funParams.size() != rhs.funParams.size())
    return false;

  for (int i = 0; i < funParams.size(); i++)
    if (*funParams[i] != *rhs.funParams[i])
      return false;

  /* Check Block*/

  return true;
}

bool FunDecl::operator!=(const FunDecl &rhs) const { return !(*this == rhs); }

std::string FunDecl::getIdentifier() const { return funName; }

void FunDecl::accept(ASTVisitor<void> &v) { return v.visit(*this); }

std::string FunDecl::accept(ASTVisitor<std::string> &v) { return v.visit(*this); }

std::shared_ptr<Type> FunDecl::accept(ASTVisitor<std::shared_ptr<Type>> &v) {
  return v.visit(*this);
}

MIPS::Register FunDecl::accept(ASTVisitor<MIPS::Register> &v) {
  return v.visit(*this);
}

X86::Register FunDecl::accept(ASTVisitor<X86::Register> &v) {
  return v.visit(*this);
}

std::string FunDecl::astClass() const { return "FunDecl"; }