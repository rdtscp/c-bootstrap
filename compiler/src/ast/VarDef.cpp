#include "../../include/ast/VarDef.h"

using namespace ACC;

VarDef::VarDef(const atl::shared_ptr<Type> &type, const atl::string &identifer,
               const atl::shared_ptr<Literal> &value, bool isExtern)
    : VarDecl(type, identifer), value(value) {}

atl::shared_ptr<VarDecl> VarDef::getptr() {
  return VarDecl::shared_from_this();
}

bool VarDef::operator==(Decl &rhs) const {
  if (rhs.astClass() == astClass())
    return *this == *static_cast<VarDef *>(&rhs);
  return false;
}

bool VarDef::operator!=(Decl &rhs) const { return !(*this == rhs); }

bool VarDef::operator==(const VarDef &rhs) const {
  if (*type != *rhs.type)
    return false;

  if (identifer != rhs.identifer)
    return false;

  return true;
}

bool VarDef::operator!=(const VarDef &rhs) const { return !(*this == rhs); }

atl::string VarDef::getIdentifier() const { return identifer; }

void VarDef::accept(ASTVisitor<void> &v) { return v.visit(*this); }

atl::string VarDef::accept(ASTVisitor<atl::string> &v) {
  return v.visit(*this);
}

atl::shared_ptr<Type> VarDef::accept(ASTVisitor<atl::shared_ptr<Type>> &v) {
  return v.visit(*this);
}

MIPS::Register VarDef::accept(ASTVisitor<MIPS::Register> &v) {
  return v.visit(*this);
}

atl::shared_ptr<X86::Operand>
VarDef::accept(ASTVisitor<atl::shared_ptr<X86::Operand>> &v) {
  return v.visit(*this);
}

atl::shared_ptr<ASTNode>
VarDef::accept(ASTVisitor<atl::shared_ptr<ASTNode>> &v) {
  return v.visit(*this);
}

atl::string VarDef::astClass() const { return "VarDef"; }