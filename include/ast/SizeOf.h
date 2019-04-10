#pragma once

#include "Expr.h"
#include "Type.h"

namespace ACC {

class SizeOf : public Expr, public atl::enable_shared_from_this<SizeOf> {

public:
  atl::shared_ptr<Type> type;

  SizeOf(const atl::shared_ptr<Type> &p_type);

  bool operator==(Expr &rhs) const override;
  bool operator!=(Expr &rhs) const override;

  bool operator==(const SizeOf &rhs) const;
  bool operator!=(const SizeOf &rhs) const;

  atl::shared_ptr<SizeOf> getptr() { return shared_from_this(); }

  atl::string astClass() const override { return "SizeOf"; }

  VISITOR_ACCEPTORS
};

} // namespace ACC
