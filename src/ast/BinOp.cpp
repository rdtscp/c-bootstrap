#include "../../include/ast/BinOp.h"

using namespace ACC;

BinOp::BinOp(std::shared_ptr<Expr> lhs, Op operation, std::shared_ptr<Expr> rhs)
    : lhs(lhs), operation(operation), rhs(rhs) {}

std::shared_ptr<BinOp> BinOp::getptr() { return shared_from_this(); }

void BinOp::accept(ASTVisitor<void> &v) { return v.visit(*this); }

std::string BinOp::accept(ASTVisitor<std::string> &v) { return v.visit(*this); }

std::shared_ptr<Type> BinOp::accept(ASTVisitor<std::shared_ptr<Type>> &v) {
  return v.visit(*this);
}

MIPS::Register BinOp::accept(ASTVisitor<MIPS::Register> &v) {
  return v.visit(*this);
}

std::shared_ptr<X86::Operand>
BinOp::accept(ASTVisitor<std::shared_ptr<X86::Operand>> &v) {
  return v.visit(*this);
}

std::shared_ptr<ASTNode>
BinOp::accept(ASTVisitor<std::shared_ptr<ASTNode>> &v) {
  return v.visit(*this);
}

std::string BinOp::astClass() const { return "BinOp"; }

std::string BinOp::opToStr(const Op op) const {
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
