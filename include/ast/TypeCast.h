#pragma once

#include "Expr.h"
#include "Type.h"

namespace ACC {

class TypeCast : public Expr, public atl::enable_shared_from_this<TypeCast> {

public:
  atl::shared_ptr<Type> type;
  atl::shared_ptr<Expr> expr;

  TypeCast(const atl::shared_ptr<Type> &p_type,
           const atl::shared_ptr<Expr> &p_expr);

  atl::string getSignature() const override;

  bool operator==(Expr &rhs) const override;
  bool operator!=(Expr &rhs) const override;

  bool operator==(const TypeCast &rhs) const;
  bool operator!=(const TypeCast &rhs) const;

  atl::shared_ptr<TypeCast> getptr() { return shared_from_this(); }

  atl::string astClass() const override { return "TypeCast"; }

  VISITOR_ACCEPTORS
};

} // namespace ACC
