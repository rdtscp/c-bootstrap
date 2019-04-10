#pragma once

#include "Expr.h"
#include "VarExpr.h"

namespace ACC {

class PrefixOp : public Expr, public atl::enable_shared_from_this<PrefixOp> {
public:
  enum class Op { INC, DEC };

  Op operation;
  atl::shared_ptr<VarExpr> variable;

  PrefixOp(const Op p_operation, const atl::shared_ptr<VarExpr> &p_variable);
  virtual ~PrefixOp() {}

  bool operator==(Expr &rhs) const override;
  bool operator!=(Expr &rhs) const override;

  bool operator==(const PrefixOp &rhs) const;
  bool operator!=(const PrefixOp &rhs) const;

  atl::shared_ptr<PrefixOp> getptr() { return shared_from_this(); }

  atl::string astClass() const override { return "PrefixOp"; }

  VISITOR_ACCEPTORS
};

} // namespace ACC
