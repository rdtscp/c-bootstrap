#include "../../include/ast/FunCall.h"

using namespace ACC;

FunCall::FunCall(const atl::shared_ptr<Identifier> &p_funIdentifier,
                 const atl::vector<atl::shared_ptr<Expr>> &p_funArgs)
    : funIdentifier(p_funIdentifier), funArgs(p_funArgs) {}

atl::shared_ptr<FunCall> FunCall::getptr() { return shared_from_this(); }

bool FunCall::operator==(Expr &rhs) const {
  if (rhs.astClass() == astClass())
    return *this == *static_cast<FunCall *>(&rhs);
  return false;
}

bool FunCall::operator!=(Expr &rhs) const { return !(*this == rhs); }

bool FunCall::operator==(const FunCall &rhs) const {
  if (*funIdentifier != *rhs.funIdentifier)
    return false;

  if (funArgs.size() != rhs.funArgs.size())
    return false;

  for (int i = 0; i < funArgs.size(); ++i) {
    if (*funArgs[i] != *rhs.funArgs[i])
      return false;
  }

  return true;
}

bool FunCall::operator!=(const FunCall &rhs) const { return !(*this == rhs); }

void FunCall::accept(ASTVisitor<void> &v) { return v.visit(*this); }

atl::string FunCall::accept(ASTVisitor<atl::string> &v) {
  return v.visit(*this);
}

atl::shared_ptr<Type> FunCall::accept(ASTVisitor<atl::shared_ptr<Type>> &v) {
  return v.visit(*this);
}

MIPS::Register FunCall::accept(ASTVisitor<MIPS::Register> &v) {
  return v.visit(*this);
}

atl::shared_ptr<X86::Operand>
FunCall::accept(ASTVisitor<atl::shared_ptr<X86::Operand>> &v) {
  return v.visit(*this);
}

atl::shared_ptr<ASTNode>
FunCall::accept(ASTVisitor<atl::shared_ptr<ASTNode>> &v) {
  return v.visit(*this);
}

atl::string FunCall::astClass() const { return "FunCall"; }