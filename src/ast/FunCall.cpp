#include "ast/FunCall.h"

using namespace ACC;

FunCall::FunCall(const atl::shared_ptr<Identifier> &p_funIdentifier,
                 const atl::vector<atl::shared_ptr<Expr>> &p_funArgs)
    : funIdentifier(p_funIdentifier), funArgs(p_funArgs) {}

atl::string FunCall::getSignature() const {
  atl::string output = funIdentifier->toString() + "(";
  const unsigned int numArgs = funArgs.size();
  for (unsigned int idx = 0u; idx < numArgs; ++idx) {
    output += funArgs[idx]->getSignature();
    if (idx < numArgs - 1u)
      output += ", ";
  }
  output += ")";
  return output;
}

bool FunCall::operator==(Expr &rhs) const {
  if (rhs.astClass() == astClass())
    return *this == *static_cast<FunCall *>(&rhs);
  return false;
}

bool FunCall::operator!=(Expr &rhs) const { return !(*this == rhs); }

bool FunCall::operator==(const FunCall &rhs) const {
  if (*funIdentifier != *rhs.funIdentifier)
    return false;

  if (funArgs.size() != rhs.funArgs.size())
    return false;

  for (unsigned int i = 0u; i < funArgs.size(); ++i) {
    if (*funArgs[i] != *rhs.funArgs[i])
      return false;
  }

  return true;
}

bool FunCall::operator!=(const FunCall &rhs) const { return !(*this == rhs); }
