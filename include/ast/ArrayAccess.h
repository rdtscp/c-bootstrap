#pragma once

#include "Expr.h"

namespace ACC {

class ArrayAccess : public Expr,
                    public atl::enable_shared_from_this<ArrayAccess> {

public:
  atl::shared_ptr<Expr> array;
  atl::shared_ptr<Expr> index;

  ArrayAccess(const atl::shared_ptr<Expr> &p_array,
              const atl::shared_ptr<Expr> &p_index);

  bool operator==(Expr &rhs) const override;
  bool operator!=(Expr &rhs) const override;

  bool operator==(const ArrayAccess &rhs) const;
  bool operator!=(const ArrayAccess &rhs) const;

  atl::shared_ptr<ArrayAccess> getptr() { return shared_from_this(); }

  atl::string astClass() const override { return "ArrayAccess"; }

  VISITOR_ACCEPTORS
};

} // namespace ACC