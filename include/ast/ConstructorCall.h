#pragma once

#include "ConstructorDecl.h"
#include "Expr.h"
#include "Identifier.h"
#include "Type.h"

namespace ACC {

class ConstructorCall : public Expr,
                        public atl::enable_shared_from_this<ConstructorCall> {

public:
  atl::weak_ptr<VarDecl> objectToConstruct;
  atl::shared_ptr<Identifier> constructorIdentifier;
  atl::vector<atl::shared_ptr<Expr>> constructorArgs;
  atl::weak_ptr<ConstructorDecl> constructorDecl;

  ConstructorCall(const atl::shared_ptr<Identifier> &p_ctorIdentifier,
                  const atl::vector<atl::shared_ptr<Expr>> &p_ctorArgs);

  bool operator==(Expr &rhs) const override;
  bool operator!=(Expr &rhs) const override;

  bool operator==(const ConstructorCall &rhs) const;
  bool operator!=(const ConstructorCall &rhs) const;

  atl::shared_ptr<ConstructorCall> getptr() { return shared_from_this(); }

  atl::string astClass() const override { return "ConstructorCall"; }

  VISITOR_ACCEPTORS
};

} // namespace ACC
