#pragma once

#include "Expr.h"
#include "FunDecl.h"
#include "Stmt.h"
#include "Type.h"

namespace ACC {

class Assign : public Stmt, public atl::enable_shared_from_this<Assign> {

public:
  atl::shared_ptr<Expr> lhs;
  atl::shared_ptr<Expr> rhs;
  atl::shared_ptr<FunDecl> assignOverload;

  Assign(const atl::shared_ptr<Expr> &p_lhs,
         const atl::shared_ptr<Expr> &p_rhs);

  bool operator==(const Assign &rhs) const;
  bool operator!=(const Assign &rhs) const;

  bool operator==(const Stmt &rhs) const override;
  bool operator!=(const Stmt &rhs) const override;

  atl::shared_ptr<Assign> getptr() { return shared_from_this(); }

  atl::string astClass() const override { return "Assign"; }

  VISITOR_ACCEPTORS
};

} // namespace ACC
