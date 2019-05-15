#pragma once

#include "Expr.h"
#include "Type.h"

namespace ACC {

class ValueAt : public Expr, public atl::enable_shared_from_this<ValueAt> {

public:
  atl::shared_ptr<Expr> derefExpr;

  ValueAt(const atl::shared_ptr<Expr> &p_derefExpr);

  
  bool operator==(Expr &rhs) const override;
  bool operator!=(Expr &rhs) const override;

  bool operator==(const ValueAt &rhs) const;
  bool operator!=(const ValueAt &rhs) const;

  atl::shared_ptr<ValueAt> getptr() { return shared_from_this(); }

  atl::string astClass() const override { return "ValueAt"; }

  VISITOR_ACCEPTORS
};

} // namespace ACC
