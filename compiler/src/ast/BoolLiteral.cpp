#include "../../include/ast/BoolLiteral.h"

using namespace ACC;

BoolLiteral::BoolLiteral(const atl::string &literal) : Literal(literal) {}

atl::shared_ptr<BoolLiteral> BoolLiteral::getptr() {
  return shared_from_this();
}

atl::string BoolLiteral::getLiteral() const {
  if (value.find('x') != atl::string::npos) {
    unsigned int intValue = std::stoul(std::string(value.c_str()), nullptr, 16);
    return atl::to_string(intValue);
  }
  return value;
}

bool BoolLiteral::operator==(Expr &rhs) const {
  if (rhs.astClass() == astClass())
    return *this == *static_cast<BoolLiteral *>(&rhs);
  return false;
}

bool BoolLiteral::operator!=(Expr &rhs) const { return !(*this == rhs); }

bool BoolLiteral::operator==(Literal &rhs) const {
  if (rhs.astClass() == astClass())
    return *this == *static_cast<BoolLiteral *>(&rhs);
  return false;
}

bool BoolLiteral::operator!=(Literal &rhs) const { return !(*this == rhs); }

bool BoolLiteral::operator==(const BoolLiteral &rhs) const {
  if (value != rhs.value)
    return false;

  return true;
}

bool BoolLiteral::operator!=(const BoolLiteral &rhs) const {
  return !(*this == rhs);
}

void BoolLiteral::accept(ASTVisitor<void> &v) { return v.visit(*this); }

atl::string BoolLiteral::accept(ASTVisitor<atl::string> &v) {
  return v.visit(*this);
}

atl::shared_ptr<Type>
BoolLiteral::accept(ASTVisitor<atl::shared_ptr<Type>> &v) {
  return v.visit(*this);
}

MIPS::Register BoolLiteral::accept(ASTVisitor<MIPS::Register> &v) {
  return v.visit(*this);
}

atl::shared_ptr<X86::Operand>
BoolLiteral::accept(ASTVisitor<atl::shared_ptr<X86::Operand>> &v) {
  return v.visit(*this);
}

atl::shared_ptr<ASTNode>
BoolLiteral::accept(ASTVisitor<atl::shared_ptr<ASTNode>> &v) {
  return v.visit(*this);
}

atl::string BoolLiteral::astClass() const { return "BoolLiteral"; }