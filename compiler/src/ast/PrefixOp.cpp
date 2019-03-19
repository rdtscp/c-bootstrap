
#include "../../include/ast/PrefixOp.h"

using namespace ACC;

PrefixOp::PrefixOp(const Op p_operation,
                   const atl::shared_ptr<VarExpr> &p_variable)
    : operation(p_operation), variable(p_variable) {}

atl::shared_ptr<PrefixOp> PrefixOp::getptr() { return shared_from_this(); }

bool PrefixOp::operator==(Expr &rhs) const {
  if (rhs.astClass() == astClass())
    return *this == *static_cast<PrefixOp *>(&rhs);
  return false;
}

bool PrefixOp::operator!=(Expr &rhs) const { return !(*this == rhs); }

bool PrefixOp::operator==(const PrefixOp &rhs) const {
  if (operation != rhs.operation)
    return false;

  if (*variable != *rhs.variable)
    return false;

  return true;
}

bool PrefixOp::operator!=(const PrefixOp &rhs) const { return !(*this == rhs); }

void PrefixOp::accept(ASTVisitor<void> &v) { return v.visit(*this); }

atl::string PrefixOp::accept(ASTVisitor<atl::string> &v) {
  return v.visit(*this);
}

atl::shared_ptr<Type> PrefixOp::accept(ASTVisitor<atl::shared_ptr<Type>> &v) {
  return v.visit(*this);
}

MIPS::Register PrefixOp::accept(ASTVisitor<MIPS::Register> &v) {
  return v.visit(*this);
}

atl::shared_ptr<X86::Operand>
PrefixOp::accept(ASTVisitor<atl::shared_ptr<X86::Operand>> &v) {
  return v.visit(*this);
}

atl::shared_ptr<ASTNode>
PrefixOp::accept(ASTVisitor<atl::shared_ptr<ASTNode>> &v) {
  return v.visit(*this);
}

atl::string PrefixOp::astClass() const { return "PrefixOp"; }