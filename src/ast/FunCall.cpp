#include "../../include/ast/FunCall.h"

using namespace ACC;

FunCall::FunCall(std::string funName,
                 std::vector<std::shared_ptr<Expr>> funArgs)
    : funName(funName), funArgs(funArgs) {}

void FunCall::accept(ASTVisitor<void> &v) { return v.visit(*this); }

std::string FunCall::accept(ASTVisitor<std::string> &v) {
  return v.visit(*this);
}

std::shared_ptr<Type> FunCall::accept(ASTVisitor<std::shared_ptr<Type>> &v) {
  return v.visit(*this);
}

MIPS::Register FunCall::accept(ASTVisitor<MIPS::Register> &v) {
  return v.visit(*this);
}

std::shared_ptr<X86::Operand> FunCall::accept(ASTVisitor<std::shared_ptr<X86::Operand>> &v) {
  return v.visit(*this);
}

std::string FunCall::astClass() const { return "FunCall"; }