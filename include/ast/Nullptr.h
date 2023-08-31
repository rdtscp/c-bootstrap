#pragma once

#include "Expr.h"
#include "Type.h"

namespace ACC {

class Nullptr : public Expr, public atl::enable_shared_from_this<Nullptr> {

public:
  Nullptr();

  bool operator==(Expr &rhs) const override;
  bool operator!=(Expr &rhs) const override;

  bool operator==(const Nullptr &rhs) const;
  bool operator!=(const Nullptr &rhs) const;

  bool operator==(const Stmt &rhs) const override;
  bool operator!=(const Stmt &rhs) const override;

  atl::shared_ptr<Nullptr> getptr() { return shared_from_this(); }

  atl::string astClass() const override { return "Nullptr"; };

  VISITOR_ACCEPTORS
};

} // namespace ACC
