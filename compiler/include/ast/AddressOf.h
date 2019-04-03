#pragma once

#include "Expr.h"

namespace ACC {

class AddressOf : public Expr, public atl::enable_shared_from_this<AddressOf> {

public:
  atl::shared_ptr<Expr> addressOfExpr;

  AddressOf(const atl::shared_ptr<Expr> &p_addressOfExpr);

  bool operator==(Expr &rhs) const override;
  bool operator!=(Expr &rhs) const override;

  bool operator==(const AddressOf &rhs) const;
  bool operator!=(const AddressOf &rhs) const;

  atl::shared_ptr<AddressOf> getptr() { return shared_from_this(); }

  atl::string astClass() const override { return "AddressOf"; };

  VISITOR_ACCEPTORS
};

} // namespace ACC