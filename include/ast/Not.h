#pragma once

#include "Expr.h"
#include "Type.h"

namespace ACC {

class Not : public Expr,
            public atl::enable_shared_from_this<Not> {

public:
  atl::shared_ptr<Expr> expr;

  Not(const atl::shared_ptr<Expr> &p_expr);

  bool operator==(Expr &rhs) const override;
  bool operator!=(Expr &rhs) const override;

  bool operator==(const Not &rhs) const;
  bool operator!=(const Not &rhs) const;

  bool operator==(const Stmt &rhs) const override;
  bool operator!=(const Stmt &rhs) const override;

  atl::shared_ptr<Not> getptr() { return shared_from_this(); }

  atl::string astClass() const override { return "Not"; }

  VISITOR_ACCEPTORS
};

} // namespace ACC
