#include "../../include/ast/VarDecl.h"

using namespace ACC;

VarDecl::VarDecl(std::shared_ptr<Type> type, std::string p_identifer,
                 bool isExtern)
    : identifer(p_identifer), type(type), isExtern(isExtern) {}

std::shared_ptr<VarDecl::Decl> VarDecl::getptr() { return shared_from_this(); }

bool VarDecl::operator==(Decl &rhs) const {
  if (rhs.astClass() == astClass())
    return *this == *static_cast<VarDecl *>(&rhs);
  return false;
}

bool VarDecl::operator!=(Decl &rhs) const { return !(*this == rhs); }

bool VarDecl::operator==(const VarDecl &rhs) const {
  if (*type != *rhs.type)
    return false;

  if (identifer != rhs.identifer)
    return false;

  return true;
}
bool VarDecl::operator!=(const VarDecl &rhs) const { return !(*this == rhs); }

int VarDecl::getBytes() const { return type->getBytes(); }

std::string VarDecl::getIdentifier() const { return identifer; }

void VarDecl::accept(ASTVisitor<void> &v) { return v.visit(*this); }

std::string VarDecl::accept(ASTVisitor<std::string> &v) {
  return v.visit(*this);
}

std::shared_ptr<Type> VarDecl::accept(ASTVisitor<std::shared_ptr<Type>> &v) {
  return v.visit(*this);
}

MIPS::Register VarDecl::accept(ASTVisitor<MIPS::Register> &v) {
  return v.visit(*this);
}

std::shared_ptr<X86::Operand>
VarDecl::accept(ASTVisitor<std::shared_ptr<X86::Operand>> &v) {
  return v.visit(*this);
}

std::shared_ptr<ASTNode>
VarDecl::accept(ASTVisitor<std::shared_ptr<ASTNode>> &v) {
  return v.visit(*this);
}

std::string VarDecl::astClass() const { return "VarDecl"; }