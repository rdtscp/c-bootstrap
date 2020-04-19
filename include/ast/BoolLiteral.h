#pragma once

#include "Expr.h"
#include "Literal.h"
#include "Type.h"

namespace ACC {

class BoolLiteral : public Literal,
                    public atl::enable_shared_from_this<BoolLiteral> {
public:
  BoolLiteral(const atl::string &p_literal);

  bool operator==(const BoolLiteral &rhs) const;
  bool operator!=(const BoolLiteral &rhs) const;

  bool operator==(Expr &rhs) const override;
  bool operator!=(Expr &rhs) const override;

  bool operator==(Literal &rhs) const override;
  bool operator!=(Literal &rhs) const override;

  bool operator==(const Stmt &rhs) const override;
  bool operator!=(const Stmt &rhs) const override;

  atl::shared_ptr<BoolLiteral> getptr() { return shared_from_this(); }

  atl::string astClass() const override { return "BoolLiteral"; }

  VISITOR_ACCEPTORS
};

} // namespace ACC
