#include "../../include/ast/CharLiteral.h"

using namespace ACC;

CharLiteral::CharLiteral(const atl::string &p_literal) : Literal(p_literal) {}

atl::shared_ptr<CharLiteral> CharLiteral::getptr() {
  return shared_from_this();
}

atl::string CharLiteral::getLiteral() const { return value; }

bool CharLiteral::operator==(Expr &rhs) const {
  if (rhs.astClass() == astClass())
    return *this == *static_cast<CharLiteral *>(&rhs);
  return false;
}

bool CharLiteral::operator!=(Expr &rhs) const { return !(*this == rhs); }

bool CharLiteral::operator==(Literal &rhs) const {
  if (rhs.astClass() == astClass())
    return *this == *static_cast<CharLiteral *>(&rhs);
  return false;
}

bool CharLiteral::operator!=(Literal &rhs) const { return !(*this == rhs); }

bool CharLiteral::operator==(const CharLiteral &rhs) const {
  if (value != rhs.value)
    return false;

  return true;
}

bool CharLiteral::operator!=(const CharLiteral &rhs) const {
  return !(*this == rhs);
}

void CharLiteral::accept(ASTVisitor<void> &v) { return v.visit(*this); }

atl::string CharLiteral::accept(ASTVisitor<atl::string> &v) {
  return v.visit(*this);
}

atl::shared_ptr<Type>
CharLiteral::accept(ASTVisitor<atl::shared_ptr<Type>> &v) {
  return v.visit(*this);
}

MIPS::Register CharLiteral::accept(ASTVisitor<MIPS::Register> &v) {
  return v.visit(*this);
}

atl::shared_ptr<X86::Operand>
CharLiteral::accept(ASTVisitor<atl::shared_ptr<X86::Operand>> &v) {
  return v.visit(*this);
}

atl::shared_ptr<ASTNode>
CharLiteral::accept(ASTVisitor<atl::shared_ptr<ASTNode>> &v) {
  return v.visit(*this);
}

atl::string CharLiteral::astClass() const { return "CharLiteral"; }