#pragma once

#include "Expr.h"
#include "Type.h"

namespace ACC {

class StaticCast : public Expr,
                   public atl::enable_shared_from_this<StaticCast> {

public:
  atl::shared_ptr<Type> type;
  atl::shared_ptr<Expr> expr;

  StaticCast(const atl::shared_ptr<Type> &p_type,
             const atl::shared_ptr<Expr> &p_expr);

  bool operator==(Expr &rhs) const override;
  bool operator!=(Expr &rhs) const override;

  bool operator==(const StaticCast &rhs) const;
  bool operator!=(const StaticCast &rhs) const;

  bool operator==(const Stmt &rhs) const override;
  bool operator!=(const Stmt &rhs) const override;

  atl::shared_ptr<StaticCast> getptr() { return shared_from_this(); }

  atl::string astClass() const override { return "StaticCast"; }

  VISITOR_ACCEPTORS
};

} // namespace ACC
