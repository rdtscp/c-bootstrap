#pragma once

namespace ACC {

enum class Op {
  ADD,
  SUB,
  MUL,
  DIV,
  MOD,
  GT,
  LT,
  GE,
  LE,
  NE,
  EQ,
  OR,
  AND,
  ASSIGNADD
};

static atl::string opToStr(Op op) {
  switch (op) {
  case Op::ADD:
    return "+";
  case Op::SUB:
    return "-";
  case Op::MUL:
    return "*";
  case Op::DIV:
    return "/";
  case Op::MOD:
    return "%";
  case Op::GT:
    return ">";
  case Op::LT:
    return "<";
  case Op::GE:
    return ">=";
  case Op::LE:
    return "<=";
  case Op::EQ:
    return "==";
  case Op::NE:
    return "!=";
  case Op::OR:
    return "||";
  case Op::AND:
    return "&&";
  case Op::ASSIGNADD:
    return "+=";
  default:
    throw "Internal Error: Unknown Op in opToStr()";
  }
} // namespace ACC

} // namespace ACC
