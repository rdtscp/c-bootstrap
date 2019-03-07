#include "../../include/ast/IntLiteral.h"

using namespace ACC;

IntLiteral::IntLiteral(const atl::string &literal) : Literal(literal) {}

atl::shared_ptr<IntLiteral> IntLiteral::getptr() { return shared_from_this(); }

atl::string IntLiteral::getLiteral() const {
  if (value.find('x') != atl::string::npos) {
    unsigned int intValue = std::stoul(std::string(value.c_str()), nullptr, 16);
    return atl::to_string(intValue);
  }
  return value;
}

bool IntLiteral::operator==(Expr &rhs) const {
  if (rhs.astClass() == astClass())
    return *this == *static_cast<IntLiteral *>(&rhs);
  return false;
}

bool IntLiteral::operator!=(Expr &rhs) const { return !(*this == rhs); }

bool IntLiteral::operator==(Literal &rhs) const {
  if (rhs.astClass() == astClass())
    return *this == *static_cast<IntLiteral *>(&rhs);
  return false;
}

bool IntLiteral::operator!=(Literal &rhs) const { return !(*this == rhs); }

bool IntLiteral::operator==(const IntLiteral &rhs) const {
  if (value != rhs.value)
    return false;

  return true;
}

bool IntLiteral::operator!=(const IntLiteral &rhs) const {
  return !(*this == rhs);
}

void IntLiteral::accept(ASTVisitor<void> &v) { return v.visit(*this); }

atl::string IntLiteral::accept(ASTVisitor<atl::string> &v) {
  return v.visit(*this);
}

atl::shared_ptr<Type> IntLiteral::accept(ASTVisitor<atl::shared_ptr<Type>> &v) {
  return v.visit(*this);
}

MIPS::Register IntLiteral::accept(ASTVisitor<MIPS::Register> &v) {
  return v.visit(*this);
}

atl::shared_ptr<X86::Operand>
IntLiteral::accept(ASTVisitor<atl::shared_ptr<X86::Operand>> &v) {
  return v.visit(*this);
}

atl::shared_ptr<ASTNode>
IntLiteral::accept(ASTVisitor<atl::shared_ptr<ASTNode>> &v) {
  return v.visit(*this);
}

atl::string IntLiteral::astClass() const { return "IntLiteral"; }