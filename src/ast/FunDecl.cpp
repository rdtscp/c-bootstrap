#include "ast/FunDecl.h"

using namespace ACC;

FunDecl::FunDecl(const atl::set<FunModifiers> &p_funModifiers,
                 const atl::shared_ptr<Identifier> &p_funIdentifier,
                 const atl::vector<atl::shared_ptr<VarDecl>> &p_funParams,
                 const atl::shared_ptr<Type> &p_funType)
    : funModifiers(p_funModifiers), funIdentifier(p_funIdentifier),
      funParams(p_funParams), funType(p_funType) {}

atl::shared_ptr<Identifier> FunDecl::getIdentifier() const {
  return funIdentifier;
}

atl::string FunDecl::getSignature() const {
  atl::string output = getIdentifier()->toString();

  output += "(";

  for (unsigned int idx = 0u; idx < funParams.size(); ++idx) {
    output += funParams[idx]->type->getSignature();
    if (idx != funParams.size() - 1u) {
      output += ", ";
    }
  }

  output += ")";

  return output;
}

bool FunDecl::operator==(Decl &rhs) const {
  if (rhs.astClass() == astClass())
    return *this == *static_cast<FunDecl *>(&rhs);
  return false;
}

bool FunDecl::operator!=(Decl &rhs) const { return !(*this == rhs); }

bool FunDecl::operator==(const FunDecl &rhs) const {
  if (funModifiers != rhs.funModifiers)
    return false;

  if (*funType != *rhs.funType)
    return false;

  if (*funIdentifier != *rhs.funIdentifier)
    return false;

  if (funParams.size() != rhs.funParams.size())
    return false;

  for (unsigned int i = 0u; i < funParams.size(); ++i)
    if (*funParams[i]->type != *rhs.funParams[i]->type)
      return false;

  return true;
}

bool FunDecl::operator!=(const FunDecl &rhs) const { return !(*this == rhs); }
