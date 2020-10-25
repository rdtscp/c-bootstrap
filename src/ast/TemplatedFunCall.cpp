#include "ast/TemplatedFunCall.h"

using namespace ACC;

TemplatedFunCall::TemplatedFunCall(
    const atl::shared_ptr<Identifier> &p_funIdentifier,
    const atl::vector<atl::shared_ptr<Expr>> &p_funArgs,
    const atl::vector<atl::shared_ptr<Type>> &p_templateArgs)
    : FunCall(p_funIdentifier, p_funArgs), templateArgs(p_templateArgs) {}

bool TemplatedFunCall::operator==(Expr &rhs) const {
  if (rhs.astClass() == astClass())
    return *this == *static_cast<TemplatedFunCall *>(&rhs);
  return false;
}

bool TemplatedFunCall::operator!=(Expr &rhs) const { return !(*this == rhs); }

bool TemplatedFunCall::operator==(const TemplatedFunCall &rhs) const {
  if (*funIdentifier != *rhs.funIdentifier)
    return false;

  if (funArgs.size() != rhs.funArgs.size())
    return false;

  for (unsigned int i = 0; i < funArgs.size(); ++i) {
    if (*funArgs[i] != *rhs.funArgs[i])
      return false;
  }

  if (templateArgs.size() != rhs.templateArgs.size())
    return false;

  for (unsigned int i = 0; i < templateArgs.size(); ++i) {
    if (*templateArgs[i] != *rhs.templateArgs[i])
      return false;
  }

  return true;
}

bool TemplatedFunCall::operator!=(const TemplatedFunCall &rhs) const {
  return !(*this == rhs);
}

bool TemplatedFunCall::operator==(const Stmt &rhs) const {
  if (rhs.astClass() == astClass())
    return *this == *static_cast<const TemplatedFunCall *>(&rhs);
  return false;
}

bool TemplatedFunCall::operator!=(const Stmt &rhs) const {
  return !(*this == rhs);
}
