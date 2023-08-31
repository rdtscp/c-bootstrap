#pragma once

#include "Expr.h"
#include "Scope.h"
#include "Type.h"
#include "VarDecl.h"

namespace ACC {

class For : public Stmt,
            public Scope,
            public atl::enable_shared_from_this<For> {

public:
  atl::shared_ptr<VarDecl> initialVarDecl;
  atl::shared_ptr<Expr> condition;
  atl::shared_ptr<Expr> endBodyExpr;
  atl::shared_ptr<Stmt> body;

  For(const atl::shared_ptr<VarDecl> &p_initialVarDecl,
      const atl::shared_ptr<Expr> &p_condition,
      const atl::shared_ptr<Expr> &p_endBodyExpr,
      const atl::shared_ptr<Stmt> &p_body);

  bool operator==(const For &rhs) const;
  bool operator!=(const For &rhs) const;

  bool operator==(const Stmt &rhs) const override;
  bool operator!=(const Stmt &rhs) const override;

  atl::shared_ptr<For> getptr() { return shared_from_this(); }

  atl::string astClass() const override { return "For"; }

  SCOPE_OVERRIDES

  VISITOR_ACCEPTORS
};

} // namespace ACC
