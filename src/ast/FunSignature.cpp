#include "ast/FunSignature.h"

#include "ast/ReferenceType.h"

using namespace ACC;

FunSignature::FunSignature(const atl::shared_ptr<Type> p_funReturnType,
                           const atl::shared_ptr<Identifier> p_funIdentifier,
                           const atl::vector<atl::shared_ptr<Type>> p_funArgs,
                           const atl::set<FunDecl::FunModifiers> p_funModifiers)
    : funReturnType(p_funReturnType), funIdentifier(p_funIdentifier),
      funArgs(p_funArgs), funModifiers(p_funModifiers) {}

const unsigned int FunSignature::namespaceCount() const {
  return funIdentifier->size() - 1;
}

const atl::string FunSignature::namespaceHead() const {
  return funIdentifier->head();
}

const FunSignature FunSignature::lowerNamespace() const {
  return FunSignature(funReturnType, funIdentifier->tail(), funArgs,
                      funModifiers);
}

bool FunSignature::canCall(const FunSignature &rhs) const {
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
  if (*funIdentifier != *rhs.funIdentifier)
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

  return output;
}

atl::shared_ptr<Type> collapseReferenceTypes(atl::shared_ptr<Type> type) {
  if (type->astClass() == "ReferenceType") {
    type = atl::static_pointer_cast<ReferenceType>(type)->referencedType;
    if (type->astClass() == "ReferenceType")
      type = atl::static_pointer_cast<ReferenceType>(type)->referencedType;
  }
  return type;
}

bool FunSignature::operator==(const FunSignature &rhs) const {
  // TODO: Incorporate return type?

  if (funArgs.size() != rhs.funArgs.size())
    return false;

  for (unsigned int idx = 0u; idx < funArgs.size(); ++idx) {
    const atl::shared_ptr<Type> lhsType = collapseReferenceTypes(funArgs[idx]);
    const atl::shared_ptr<Type> rhsType =
        collapseReferenceTypes(rhs.funArgs[idx]);
    // const atl::shared_ptr<Type> lhsType = funArgs[idx];
    // const atl::shared_ptr<Type> rhsType = rhs.funArgs[idx];

    if (*lhsType != *rhsType && !lhsType->canCastTo(*rhsType))
      return false;
  }

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

atl::string FunSignature::toString() const {
  atl::string output = funIdentifier->toString() + "(";

  for (unsigned int i = 0; i < funArgs.size(); ++i) {
    output += funArgs[i]->astClass();
    if (i != funArgs.size() - 1) {
      output += ", ";
    }
  }

  output += ")";
  return output;
}