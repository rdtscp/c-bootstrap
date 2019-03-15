#include "../../include/ast/EnumClassTypeDecl.h"

using namespace ACC;

EnumClassTypeDecl::EnumClassTypeDecl(const atl::string &ident,
                           const std::map<std::string, std::string> &states)
    : states(states) {}

atl::shared_ptr<EnumClassTypeDecl> EnumClassTypeDecl::getptr() {
  return shared_from_this();
}

bool EnumClassTypeDecl::operator==(Decl &rhs) const {
  if (rhs.astClass() == astClass())
    return *this == *static_cast<EnumClassTypeDecl *>(&rhs);
  return false;
}

bool EnumClassTypeDecl::operator!=(Decl &rhs) const { return !(*this == rhs); }

atl::string EnumClassTypeDecl::getIdentifier() const { return identifer; }

bool EnumClassTypeDecl::operator==(Type &rhs) const {
  if (rhs.astClass() == astClass())
    return *this == *static_cast<EnumClassTypeDecl *>(&rhs);
  return false;
}

bool EnumClassTypeDecl::operator!=(Type &rhs) const { return !(*this == rhs); }

int EnumClassTypeDecl::getBytes() const { return 4; }

bool EnumClassTypeDecl::operator==(const EnumClassTypeDecl &rhs) const {
  return states == rhs.states;
}

bool EnumClassTypeDecl::operator!=(const EnumClassTypeDecl &rhs) const {
  return !(*this == rhs);
}

void EnumClassTypeDecl::accept(ASTVisitor<void> &v) { return v.visit(*this); }

atl::string EnumClassTypeDecl::accept(ASTVisitor<atl::string> &v) {
  return v.visit(*this);
}

atl::shared_ptr<Type>
EnumClassTypeDecl::accept(ASTVisitor<atl::shared_ptr<Type>> &v) {
  return v.visit(*this);
}

MIPS::Register EnumClassTypeDecl::accept(ASTVisitor<MIPS::Register> &v) {
  return v.visit(*this);
}

atl::shared_ptr<X86::Operand>
EnumClassTypeDecl::accept(ASTVisitor<atl::shared_ptr<X86::Operand>> &v) {
  return v.visit(*this);
}

atl::shared_ptr<ASTNode>
EnumClassTypeDecl::accept(ASTVisitor<atl::shared_ptr<ASTNode>> &v) {
  return v.visit(*this);
}

atl::string EnumClassTypeDecl::astClass() const { return "EnumClassTypeDecl"; }