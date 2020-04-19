#pragma once

#include "Expr.h"
#include "Stmt.h"
#include "Type.h"

namespace ACC {

class DoWhile : public Stmt, public atl::enable_shared_from_this<DoWhile> {

public:
  atl::shared_ptr<Stmt> body;
  atl::shared_ptr<Expr> condition;

  DoWhile(const atl::shared_ptr<Stmt> &p_body,
          const atl::shared_ptr<Expr> &p_condition);

  bool operator==(const DoWhile &rhs) const;
  bool operator!=(const DoWhile &rhs) const;

  bool operator==(const Stmt &rhs) const override;
  bool operator!=(const Stmt &rhs) const override;

  atl::shared_ptr<DoWhile> getptr() { return shared_from_this(); }

  atl::string astClass() const override { return "DoWhile"; }

  VISITOR_ACCEPTORS
};

} // namespace ACC
