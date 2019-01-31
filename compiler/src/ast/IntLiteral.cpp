#include "../../include/ast/IntLiteral.h"

using namespace ACC;

IntLiteral::IntLiteral(const atl::string &literal) : value(literal) {}

std::shared_ptr<IntLiteral> IntLiteral::getptr() { return shared_from_this(); }

atl::string IntLiteral::getLiteral() const {
  if (value.find('x') != atl::string::npos) {
    unsigned int intValue = std::stoul(std::string(value.c_str()), nullptr, 16);
    return std::to_string(intValue).c_str();
  }
  return value;
}

void IntLiteral::accept(ASTVisitor<void> &v) { return v.visit(*this); }

atl::string IntLiteral::accept(ASTVisitor<atl::string> &v) {
  return v.visit(*this);
}

std::shared_ptr<Type> IntLiteral::accept(ASTVisitor<std::shared_ptr<Type>> &v) {
  return v.visit(*this);
}

MIPS::Register IntLiteral::accept(ASTVisitor<MIPS::Register> &v) {
  return v.visit(*this);
}

std::shared_ptr<X86::Operand>
IntLiteral::accept(ASTVisitor<std::shared_ptr<X86::Operand>> &v) {
  return v.visit(*this);
}

std::shared_ptr<ASTNode>
IntLiteral::accept(ASTVisitor<std::shared_ptr<ASTNode>> &v) {
  return v.visit(*this);
}

atl::string IntLiteral::astClass() const { return "IntLiteral"; }