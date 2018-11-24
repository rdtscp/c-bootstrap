#pragma once

#ifndef BINOP_H
#define BINOP_H

#include <string>

#include "Expr.h"
#include "Op.h"

namespace ACC {

class BinOp : public Expr {

public:
  std::shared_ptr<Expr> lhs;
  Op operation;
  std::shared_ptr<Expr> rhs;

  BinOp(std::shared_ptr<Expr> lhs, Op operation, std::shared_ptr<Expr> rhs)
      : lhs(lhs), operation(operation), rhs(rhs) {
    // Boilerplate Code.
  }

  bool operator==(const BinOp &d) const { return true; }

  std::string astClass() const override { return "BinOp"; }

  std::string opToStr(const Op op) const {
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
};

}; // namespace ACC

#endif