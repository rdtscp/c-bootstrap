#pragma once

#include "Decl.h"
#include "Scope.h"
#include "Type.h"
#include "VarDecl.h"

namespace ACC {

class FunSignature;

class FunDecl : public Decl,
                public Scope,
                public atl::enable_shared_from_this<FunDecl> {

public:
  enum class FunModifiers { CONST, OVERRIDE, STATIC, VIRTUAL };
  atl::set<FunModifiers> funModifiers;
  atl::shared_ptr<Identifier> funIdentifier;
  atl::vector<atl::shared_ptr<VarDecl>> funParams;
  atl::shared_ptr<Type> funType;

  int numCallers = 0;

  FunDecl(const atl::set<FunModifiers> &p_funModifiers,
          const atl::shared_ptr<Identifier> &p_funIdentifier,
          const atl::vector<atl::shared_ptr<VarDecl>> &p_funParams,
          const atl::shared_ptr<Type> &p_funType);

  atl::shared_ptr<Identifier> getIdentifier() const override;
  const FunSignature getSignature() const;

  bool operator==(Decl &rhs) const override;
  bool operator!=(Decl &rhs) const override;

  bool operator==(const FunDecl &rhs) const;
  bool operator!=(const FunDecl &rhs) const;

  atl::shared_ptr<FunDecl> getptr() { return shared_from_this(); }

  atl::string astClass() const override { return "FunDecl"; }

  SCOPE_OVERRIDES

  VISITOR_ACCEPTORS
};

} // namespace ACC
