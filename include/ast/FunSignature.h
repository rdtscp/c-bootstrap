#pragma once

#include "atl/include/shared_ptr.h"
#include "atl/include/vector.h"

#include "ast/FunDecl.h"
#include "ast/Identifier.h"
#include "ast/Type.h"

namespace ACC {

class FunSignature {
public:
  FunSignature(const atl::shared_ptr<Type> p_funReturnType,
               const atl::shared_ptr<Identifier> p_funIdentifier,
               const atl::vector<atl::shared_ptr<Type>> p_funArgs,
               const atl::set<FunDecl::FunModifiers> p_funModifiers);

  const unsigned int namespaceCount() const;

  const atl::string namespaceHead() const;

  const FunSignature lowerNamespace() const;

  bool canCall(const FunSignature &rhs) const;

  atl::string mangle() const;

  bool operator==(const FunSignature &rhs) const;
  bool operator!=(const FunSignature &rhs) const;

  atl::string toString() const;

protected:
  const atl::shared_ptr<Type> funReturnType;
  const atl::shared_ptr<Identifier> funIdentifier;
  const atl::vector<atl::shared_ptr<Type>> funArgs;
  const atl::set<FunDecl::FunModifiers> funModifiers;
};

class TemplateFunSignature : public FunSignature {
public:
  atl::vector<atl::shared_ptr<Type>> templateArgs;

  TemplateFunSignature(const atl::shared_ptr<Type> p_funReturnType,
                       const atl::shared_ptr<Identifier> p_funIdentifier,
                       const atl::vector<atl::shared_ptr<Type>> p_funArgs,
                       const atl::set<FunDecl::FunModifiers> p_funModifiers,
                       const atl::vector<atl::shared_ptr<Type>> p_templateArgs)
      : FunSignature(p_funReturnType, p_funIdentifier, p_funArgs,
                     p_funModifiers),
        templateArgs(p_templateArgs) {}
};

} // namespace ACC
