#include "../../include/ast/EnumTypeDecl.h"

using namespace ACC;

EnumTypeDecl::EnumTypeDecl(const atl::shared_ptr<Identifier> &p_identifier,
                           const std::map<std::string, std::string> &p_states)
    : identifier(p_identifier), states(p_states) {}

atl::shared_ptr<EnumTypeDecl> EnumTypeDecl::getptr() {
  return shared_from_this();
}

bool EnumTypeDecl::operator==(Decl &rhs) const {
  if (rhs.astClass() == astClass())
    return *this == *static_cast<EnumTypeDecl *>(&rhs);
  return false;
}

bool EnumTypeDecl::operator!=(Decl &rhs) const { return !(*this == rhs); }

atl::shared_ptr<Identifier> EnumTypeDecl::getIdentifier() const {
  return identifier;
}

bool EnumTypeDecl::operator==(Type &rhs) const {
  if (rhs.astClass() == astClass())
    return *this == *static_cast<EnumTypeDecl *>(&rhs);
  return false;
}

bool EnumTypeDecl::operator!=(Type &rhs) const { return !(*this == rhs); }

int EnumTypeDecl::getBytes() const { return 4; }

atl::string EnumTypeDecl::getSignature() const {
  if (identifier)
    return atl::string("enum ") + identifier->toString();
  return "enum";  
}


bool EnumTypeDecl::operator==(const EnumTypeDecl &rhs) const {
  return states == rhs.states;
}

bool EnumTypeDecl::operator!=(const EnumTypeDecl &rhs) const {
  return !(*this == rhs);
}

void EnumTypeDecl::accept(ASTVisitor<void> &v) { return v.visit(*this); }

atl::string EnumTypeDecl::accept(ASTVisitor<atl::string> &v) {
  return v.visit(*this);
}

atl::shared_ptr<Type>
EnumTypeDecl::accept(ASTVisitor<atl::shared_ptr<Type>> &v) {
  return v.visit(*this);
}

MIPS::Register EnumTypeDecl::accept(ASTVisitor<MIPS::Register> &v) {
  return v.visit(*this);
}

atl::shared_ptr<X86::Operand>
EnumTypeDecl::accept(ASTVisitor<atl::shared_ptr<X86::Operand>> &v) {
  return v.visit(*this);
}

atl::shared_ptr<ASTNode>
EnumTypeDecl::accept(ASTVisitor<atl::shared_ptr<ASTNode>> &v) {
  return v.visit(*this);
}

atl::string EnumTypeDecl::astClass() const { return "EnumTypeDecl"; }