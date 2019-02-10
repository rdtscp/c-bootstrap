#include "../../include/ast/BinOp.h"

using namespace ACC;

BinOp::BinOp(atl::shared_ptr<Expr> lhs, Op operation, atl::shared_ptr<Expr> rhs)
    : lhs(lhs), operation(operation), rhs(rhs) {}

atl::shared_ptr<BinOp> BinOp::getptr() { return shared_from_this(); }

void BinOp::accept(ASTVisitor<void> &v) { return v.visit(*this); }

atl::string BinOp::accept(ASTVisitor<atl::string> &v) { return v.visit(*this); }

atl::shared_ptr<Type> BinOp::accept(ASTVisitor<atl::shared_ptr<Type>> &v) {
  return v.visit(*this);
}

MIPS::Register BinOp::accept(ASTVisitor<MIPS::Register> &v) {
  return v.visit(*this);
}

atl::shared_ptr<X86::Operand>
BinOp::accept(ASTVisitor<atl::shared_ptr<X86::Operand>> &v) {
  return v.visit(*this);
}

atl::shared_ptr<ASTNode>
BinOp::accept(ASTVisitor<atl::shared_ptr<ASTNode>> &v) {
  return v.visit(*this);
}

atl::string BinOp::astClass() const { return "BinOp"; }

atl::string BinOp::opToStr(const Op op) const {
  switch (op) {
  case Op::ADD:
    return " + ";
  case Op::SUB:
    return " - ";
  case Op::MUL:
    return " * ";
  case Op::DIV:
    return " / ";
  case Op::MOD:
    return " % ";
  case Op::GT:
    return " > ";
  case Op::LT:
    return " < ";
  case Op::GE:
    return " >= ";
  case Op::LE:
    return " <= ";
  case Op::NE:
    return " != ";
  case Op::EQ:
    return " == ";
  case Op::OR:
    return " || ";
  case Op::AND:
    return " && ";
  }
}
