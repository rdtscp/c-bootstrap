#pragma once

#include "Expr.h"
#include "Identifier.h"

namespace ACC {

class VarExpr : public Expr, public atl::enable_shared_from_this<VarExpr> {

public:
  atl::shared_ptr<Identifier> varIdentifier;
  atl::shared_ptr<VarDecl> varDecl;

  VarExpr(const atl::shared_ptr<Identifier> &p_varIdentifier);

  bool operator==(Expr &rhs) const override;
  bool operator!=(Expr &rhs) const override;

  bool operator==(const VarExpr &rhs) const;
  bool operator!=(const VarExpr &rhs) const;

  atl::shared_ptr<VarExpr> getptr() { return shared_from_this(); }

  atl::string astClass() const override { return "VarExpr"; }

  VISITOR_ACCEPTORS
};

} // namespace ACC
