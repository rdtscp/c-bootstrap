#include "ast/FunSignature.h"

using namespace ACC;

FunSignature::FunSignature(const atl::shared_ptr<Type> p_funReturnType,
                           const atl::shared_ptr<Identifier> p_funIdentifier,
                           const atl::vector<atl::shared_ptr<Type>> p_funArgs,
                           const atl::set<FunDecl::FunModifiers> p_funModifiers)
    : funReturnType(p_funReturnType), funIdentifier(p_funIdentifier),
      funArgs(p_funArgs), funModifiers(p_funModifiers) {}

const atl::shared_ptr<Identifier> FunSignature::getIdentifier() const {
  return funIdentifier;
}

const unsigned int FunSignature::namespaceCount() const {
  return funIdentifier->namespaceCount();
}

const atl::shared_ptr<Identifier> FunSignature::namespaceHead() const {
  return funIdentifier->namespaceHead();
}

const FunSignature FunSignature::lowerNamespace() const {
  return FunSignature(funReturnType, funIdentifier->namespaceTail(), funArgs,
                      funModifiers);
}

bool FunSignature::canCall(const FunSignature &rhs) const {
  // TODO: Incorporate return type?

  // TODO: Handle when default args are provided.
  if (funArgs.size() != rhs.funArgs.size())
    return false;

  // Check args match.
  for (unsigned int idx = 0u; idx < funArgs.size(); ++idx) {
    const atl::shared_ptr<Type> lhsType = funArgs[idx];
    const atl::shared_ptr<Type> rhsType = rhs.funArgs[idx];
    if (*lhsType != *rhsType && !lhsType->canCastTo(*rhsType))
      return false;
  }

  // Check identifier.
  if (funIdentifier->value != rhs.funIdentifier->value)
    return false;

  // Check Relevant Modifiers
  const bool lhsConst = funModifiers.find(FunDecl::FunModifiers::CONST);
  const bool rhsConst = rhs.funModifiers.find(FunDecl::FunModifiers::CONST);
  if (lhsConst && !rhsConst)
    return false;

  return true;
}

atl::string FunSignature::mangle() const {
  atl::string output = funIdentifier->mangle();
  for (unsigned int idx = 0u; idx < funArgs.size(); ++idx) {
    output += "_" + funArgs[idx]->mangle() + "_";
  }
  if (funModifiers.find(FunDecl::FunModifiers::CONST))
    output += "_const";

  return output;
}

bool FunSignature::operator==(const FunSignature &rhs) const {
  // TODO: Incorporate return type?

  if (funArgs.size() != rhs.funArgs.size())
    return false;

  // Check args match.
  for (unsigned int idx = 0u; idx < funArgs.size(); ++idx)
    if (*funArgs[idx] != *rhs.funArgs[idx])
      return false;

  // Check identifier.
  if (*funIdentifier != *rhs.funIdentifier)
    return false;

  // Check Relevant Modifiers
  const bool lhsConst = funModifiers.find(FunDecl::FunModifiers::CONST);
  const bool rhsConst = rhs.funModifiers.find(FunDecl::FunModifiers::CONST);
  if (lhsConst != rhsConst)
    return false;

  return true;
}

bool FunSignature::operator!=(const FunSignature &rhs) const {
  return !(*this == rhs);
}
