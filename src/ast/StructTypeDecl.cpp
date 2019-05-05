#include "ast/StructTypeDecl.h"

using namespace ACC;

StructTypeDecl::StructTypeDecl(
    const atl::shared_ptr<StructType> &p_structType,
    const atl::vector<atl::shared_ptr<VarDecl>> &p_varDecls)
    : structType(p_structType), varDecls(p_varDecls) {}

atl::shared_ptr<Identifier> StructTypeDecl::getIdentifier() const {
  return structType->identifier;
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

  for (unsigned int i = 0; i < varDecls.size(); ++i)
    if (*varDecls[i] != *rhs.varDecls[i])
      return false;

  return true;
}

bool StructTypeDecl::operator!=(const StructTypeDecl &rhs) const {
  return !(*this == rhs);
}