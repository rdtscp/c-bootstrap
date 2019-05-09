#pragma once

#include "Expr.h"
#include "FunCall.h"
#include "Type.h"

namespace ACC {

class Allocation : public Expr,
                   public atl::enable_shared_from_this<Allocation> {

public:
  atl::shared_ptr<Type> varType;
  atl::shared_ptr<FunCall> varConstructorCall;

  Allocation(const atl::shared_ptr<Type> &p_varType);
  Allocation(const atl::shared_ptr<FunCall> &p_varConstructorCall);

  virtual ~Allocation() {}

  atl::string getSignature() const override;

  bool operator==(Expr &rhs) const override;
  bool operator!=(Expr &rhs) const override;

  bool operator==(const Allocation &rhs) const;
  bool operator!=(const Allocation &rhs) const;

  atl::shared_ptr<Allocation> getptr() { return shared_from_this(); }

  atl::string astClass() const override { return "Allocation"; }

  VISITOR_ACCEPTORS
};

} // namespace ACC
