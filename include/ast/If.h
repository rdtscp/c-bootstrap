#pragma once

#include "Expr.h"
#include "Type.h"

namespace ACC {

class If : public Stmt, public atl::enable_shared_from_this<If> {

public:
  atl::shared_ptr<Expr> ifCondition;
  atl::shared_ptr<Stmt> ifBody;
  atl::shared_ptr<Stmt> elseBody;

  If(const atl::shared_ptr<Expr> &p_ifCondition,
     const atl::shared_ptr<Stmt> &p_ifBody);

  If(const atl::shared_ptr<Expr> &p_ifCondition,
     const atl::shared_ptr<Stmt> &p_ifBody,
     const atl::shared_ptr<Stmt> &p_elseBody);

  atl::shared_ptr<If> getptr() { return shared_from_this(); }

  atl::string astClass() const override { return "If"; }

  VISITOR_ACCEPTORS
};

} // namespace ACC
