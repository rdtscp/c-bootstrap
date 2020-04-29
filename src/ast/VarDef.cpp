#include "ast/VarDef.h"

using namespace ACC;

VarDef::VarDef(const atl::shared_ptr<Type> &p_varType,
               const atl::shared_ptr<Identifier> &p_varidentifier,
               const atl::shared_ptr<Expr> &p_varValue)
    : VarDecl(p_varType, p_varidentifier), varValue(p_varValue) {}

unsigned int VarDef::getBytes() const { return type->getBytes(); }

atl::shared_ptr<Identifier> VarDef::getIdentifier() const { return identifier; }

bool VarDef::operator==(Decl &rhs) const {
  if (rhs.astClass() == astClass())
    return *this == *static_cast<VarDef *>(&rhs);
  return false;
}

bool VarDef::operator!=(Decl &rhs) const { return !(*this == rhs); }

bool VarDef::operator==(const Stmt &rhs) const {
  if (rhs.astClass() == astClass())
    return *this == *static_cast<const VarDef *>(&rhs);
  return false;
}

bool VarDef::operator!=(const Stmt &rhs) const { return !(*this == rhs); }

bool VarDef::operator==(const VarDef &rhs) const {
  if (*type != *rhs.type)
    return false;

  if (*identifier != *rhs.identifier)
    return false;

  if (*varValue != *rhs.varValue)
    return false;

  return true;
}

bool VarDef::operator!=(const VarDef &rhs) const { return !(*this == rhs); }
