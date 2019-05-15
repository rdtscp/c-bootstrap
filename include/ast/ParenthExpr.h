#pragma once

#include "Expr.h"
#include "Type.h"

namespace ACC {

class ParenthExpr : public Expr,
                    public atl::enable_shared_from_this<ParenthExpr> {

public:
  atl::shared_ptr<Expr> innerExpr;

  ParenthExpr(const atl::shared_ptr<Expr> &p_innerExpr);

  bool operator==(Expr &rhs) const override;
  bool operator!=(Expr &rhs) const override;

  bool operator==(const ParenthExpr &rhs) const;
  bool operator!=(const ParenthExpr &rhs) const;

  atl::shared_ptr<ParenthExpr> getptr() { return shared_from_this(); }

  atl::string astClass() const override { return "ParenthExpr"; }

  VISITOR_ACCEPTORS
};

} // namespace ACC
