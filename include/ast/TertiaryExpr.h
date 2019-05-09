#pragma once

#include "Expr.h"
#include "Type.h"

namespace ACC {

class TertiaryExpr : public Expr,
                     public atl::enable_shared_from_this<TertiaryExpr> {

public:
  atl::shared_ptr<Expr> tertiaryCondition;
  atl::shared_ptr<Expr> tertiaryIfBody;
  atl::shared_ptr<Expr> tertiaryElseBody;

  TertiaryExpr(const atl::shared_ptr<Expr> &p_tertiaryCondition,
               const atl::shared_ptr<Expr> &p_tertiaryIfBody,
               const atl::shared_ptr<Expr> &p_tertiaryElseBody);

  atl::string getSignature() const override;

  bool operator==(Expr &rhs) const override;
  bool operator!=(Expr &rhs) const override;

  bool operator==(const TertiaryExpr &rhs) const;
  bool operator!=(const TertiaryExpr &rhs) const;

  atl::shared_ptr<TertiaryExpr> getptr() { return shared_from_this(); }

  atl::string astClass() const override { return "TertiaryExpr"; }

  VISITOR_ACCEPTORS
};

} // namespace ACC
