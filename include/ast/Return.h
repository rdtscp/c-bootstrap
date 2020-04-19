#pragma once

#include "Expr.h"
#include "Stmt.h"
#include "Type.h"

namespace ACC {

class Return : public Stmt, public atl::enable_shared_from_this<Return> {

public:
  atl::shared_ptr<Expr> returnExpr;

  Return();
  Return(const atl::shared_ptr<Expr> &p_returnExpr);

  bool operator==(const Return &rhs) const;
  bool operator!=(const Return &rhs) const;

  bool operator==(const Stmt &rhs) const override;
  bool operator!=(const Stmt &rhs) const override;

  atl::shared_ptr<Return> getptr() { return shared_from_this(); }

  atl::string astClass() const override { return "Return"; }

  VISITOR_ACCEPTORS
};

} // namespace ACC
