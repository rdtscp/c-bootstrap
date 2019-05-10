#pragma once

#include "Expr.h"
#include "FunDef.h"
#include "VarExpr.h"

namespace ACC {

class SubscriptOp : public Expr,
                    public atl::enable_shared_from_this<SubscriptOp> {
public:
  atl::shared_ptr<VarExpr> variable;
  atl::shared_ptr<Expr> index;
  atl::shared_ptr<FunDef> operatorDef;

  SubscriptOp(const atl::shared_ptr<VarExpr> &p_variable,
              const atl::shared_ptr<Expr> &p_index);
  virtual ~SubscriptOp() {}

  atl::string getSignature() const override;

  bool operator==(Expr &rhs) const override;
  bool operator!=(Expr &rhs) const override;

  bool operator==(const SubscriptOp &rhs) const;
  bool operator!=(const SubscriptOp &rhs) const;

  atl::shared_ptr<SubscriptOp> getptr() { return shared_from_this(); }

  atl::string astClass() const override { return "SubscriptOp"; }

  VISITOR_ACCEPTORS
};

} // namespace ACC
