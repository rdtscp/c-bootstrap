#pragma once

#include "Expr.h"
#include "Stmt.h"

namespace ACC {

class Assign : public Stmt, public atl::enable_shared_from_this<Assign> {

public:
  atl::shared_ptr<Expr> lhs;
  atl::shared_ptr<Expr> rhs;

  Assign(const atl::shared_ptr<Expr> &p_lhs,
         const atl::shared_ptr<Expr> &p_rhs);

  atl::shared_ptr<Assign> getptr() { return shared_from_this(); }

  atl::string astClass() const override { return "Assign"; }

  VISITOR_ACCEPTORS
};

} // namespace ACC
