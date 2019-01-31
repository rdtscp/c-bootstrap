#include "../../include/ast/FunDecl.h"

using namespace ACC;

FunDecl::FunDecl(const atl::string &funName,
                 std::vector<std::shared_ptr<VarDecl>> funParams,
                 std::shared_ptr<Type> funType)
    : funName(funName), funParams(funParams), funType(funType) {}

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

  return true;
}

bool FunDecl::operator!=(const FunDecl &rhs) const { return !(*this == rhs); }

atl::string FunDecl::getIdentifier() const { return funName; }

void FunDecl::accept(ASTVisitor<void> &v) { return v.visit(*this); }

atl::string FunDecl::accept(ASTVisitor<atl::string> &v) {
  return v.visit(*this);
}

std::shared_ptr<Type> FunDecl::accept(ASTVisitor<std::shared_ptr<Type>> &v) {
  return v.visit(*this);
}

MIPS::Register FunDecl::accept(ASTVisitor<MIPS::Register> &v) {
  return v.visit(*this);
}

std::shared_ptr<X86::Operand>
FunDecl::accept(ASTVisitor<std::shared_ptr<X86::Operand>> &v) {
  return v.visit(*this);
}

std::shared_ptr<ASTNode>
FunDecl::accept(ASTVisitor<std::shared_ptr<ASTNode>> &v) {
  return v.visit(*this);
}

atl::string FunDecl::astClass() const { return "FunDecl"; }