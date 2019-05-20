#pragma once

#include "atl/include/shared_ptr.h"
#include "atl/include/vector.h"

#include "ast/Identifier.h"
#include "ast/Type.h"

namespace ACC {

class FunSignature {
public:
  FunSignature(const atl::shared_ptr<Type> p_funReturnType,
               const atl::shared_ptr<Identifier> p_funIdentifier,
               const atl::vector<atl::shared_ptr<Type>> p_funArgs)
      : funReturnType(p_funReturnType), funIdentifier(p_funIdentifier),
        funArgs(p_funArgs) {}

  const unsigned int namespaceCount() const {
    return funIdentifier->namespaceCount();
  }

  const atl::shared_ptr<Identifier> namespaceHead() const {
    return funIdentifier->namespaceHead();
  }

  const FunSignature lowerNamespace() const {
    return FunSignature(funReturnType, funIdentifier->namespaceTail(), funArgs);
  }

  bool operator==(const FunSignature &rhs) const {
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

    return true;
  }

  bool operator!=(const FunSignature &rhs) const { return !(*this == rhs); }

private:
  const atl::shared_ptr<Type> funReturnType;
  const atl::shared_ptr<Identifier> funIdentifier;
  const atl::vector<atl::shared_ptr<Type>> funArgs;
};

} // namespace ACC