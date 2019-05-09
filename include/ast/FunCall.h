#pragma once

#include "Expr.h"
#include "FunDecl.h"
#include "Identifier.h"
#include "Type.h"

namespace ACC {

class FunCall : public Expr, public atl::enable_shared_from_this<FunCall> {

public:
  atl::shared_ptr<Identifier> funIdentifier;
  atl::vector<atl::shared_ptr<Expr>> funArgs;
  atl::shared_ptr<FunDecl> funDecl;

  FunCall(const atl::shared_ptr<Identifier> &p_funIdentifier,
          const atl::vector<atl::shared_ptr<Expr>> &p_funArgs);

  atl::string getSignature() const override;

  bool operator==(Expr &rhs) const override;
  bool operator!=(Expr &rhs) const override;

  bool operator==(const FunCall &rhs) const;
  bool operator!=(const FunCall &rhs) const;

  atl::shared_ptr<FunCall> getptr() { return shared_from_this(); }

  atl::string astClass() const override { return "FunCall"; }

  VISITOR_ACCEPTORS
};

} // namespace ACC
