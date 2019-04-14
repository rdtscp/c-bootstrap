#pragma once

#include "Expr.h"
#include "Stmt.h"
#include "Type.h"

namespace ACC {

class While : public Stmt, public atl::enable_shared_from_this<While> {

public:
  atl::shared_ptr<Stmt> body;
  atl::shared_ptr<Expr> condition;

  While(const atl::shared_ptr<Stmt> &p_body,
        const atl::shared_ptr<Expr> &p_condition);

  atl::shared_ptr<While> getptr() { return shared_from_this(); }

  atl::string astClass() const override { return "While"; }

  VISITOR_ACCEPTORS
};

} // namespace ACC
