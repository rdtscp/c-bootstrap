#include "../../include/ast/EnumType.h"

using namespace ACC;

EnumType::EnumType(std::map<std::string, unsigned int> states)
    : states(states) {}

std::shared_ptr<EnumType> EnumType::getptr() { return shared_from_this(); }

bool EnumType::operator==(Type &t) const {
  if (t.astClass() == astClass())
    return *this == *static_cast<EnumType *>(&t);
  return false;
}

bool EnumType::operator!=(Type &t) const { return !(*this == t); }

bool EnumType::operator==(const EnumType &rhs) const {
  return states == rhs.states;
}

bool EnumType::operator!=(const EnumType &rhs) const { return !(*this == rhs); }

int EnumType::getBytes() const { return 4; }

void EnumType::accept(ASTVisitor<void> &v) { return v.visit(*this); }

std::string EnumType::accept(ASTVisitor<std::string> &v) {
  return v.visit(*this);
}

std::shared_ptr<Type> EnumType::accept(ASTVisitor<std::shared_ptr<Type>> &v) {
  return v.visit(*this);
}

MIPS::Register EnumType::accept(ASTVisitor<MIPS::Register> &v) {
  return v.visit(*this);
}

std::shared_ptr<X86::Operand>
EnumType::accept(ASTVisitor<std::shared_ptr<X86::Operand>> &v) {
  return v.visit(*this);
}

std::shared_ptr<ASTNode>
EnumType::accept(ASTVisitor<std::shared_ptr<ASTNode>> &v) {
  return v.visit(*this);
}

std::string EnumType::astClass() const { return "EnumType"; }