#include "../../include/ast/StructTypeDecl.h"

using namespace ACC;

StructTypeDecl::StructTypeDecl(
    std::shared_ptr<StructType> structType,
    const std::vector<std::shared_ptr<VarDecl>> &varDecls)
    : structType(structType), varDecls(varDecls) {}

std::shared_ptr<StructTypeDecl> StructTypeDecl::getptr() {
  return shared_from_this();
}

bool StructTypeDecl::operator==(Decl &rhs) const {
  if (rhs.astClass() == astClass())
    return *this == *static_cast<StructTypeDecl *>(&rhs);
  return false;
}

bool StructTypeDecl::operator!=(Decl &rhs) const { return !(*this == rhs); }

bool StructTypeDecl::operator==(const StructTypeDecl &rhs) const {
  if (*structType != *rhs.structType)
    return false;

  if (varDecls.size() != rhs.varDecls.size())
    return false;

  for (int i = 0; i < varDecls.size(); i++)
    if (*varDecls[i] != *rhs.varDecls[i])
      return false;

  return true;
}

bool StructTypeDecl::operator!=(const StructTypeDecl &rhs) const {
  return !(*this == rhs);
}

atl::string StructTypeDecl::getIdentifier() const {
  return atl::string("struct ") + structType->identifier;
}

void StructTypeDecl::accept(ASTVisitor<void> &v) { return v.visit(*this); }

atl::string StructTypeDecl::accept(ASTVisitor<atl::string> &v) {
  return v.visit(*this);
}

std::shared_ptr<Type>
StructTypeDecl::accept(ASTVisitor<std::shared_ptr<Type>> &v) {
  return v.visit(*this);
}

MIPS::Register StructTypeDecl::accept(ASTVisitor<MIPS::Register> &v) {
  return v.visit(*this);
}

std::shared_ptr<X86::Operand>
StructTypeDecl::accept(ASTVisitor<std::shared_ptr<X86::Operand>> &v) {
  return v.visit(*this);
}

std::shared_ptr<ASTNode>
StructTypeDecl::accept(ASTVisitor<std::shared_ptr<ASTNode>> &v) {
  return v.visit(*this);
}

atl::string StructTypeDecl::astClass() const { return "StructTypeDecl"; }