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

private:
  const atl::shared_ptr<Type> funReturnType;
  const atl::shared_ptr<Identifier> funIdentifier;
  const atl::vector<atl::shared_ptr<Type>> funArgs;
};

} // namespace ACC