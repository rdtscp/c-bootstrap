#pragma once

#include "Expr.h"
#include "Type.h"

namespace ACC {

class ArrayType : public Type, public atl::enable_shared_from_this<ArrayType> {

public:
  atl::shared_ptr<Type> type;
  atl::shared_ptr<Expr> size;

  ArrayType(const atl::shared_ptr<Type> &p_type,
            const atl::shared_ptr<Expr> &p_size);

  int getBytes() const override;

  atl::string getSignature() const override;

  bool operator==(Type &rhs) const override;
  bool operator!=(Type &rhs) const override;

  bool operator==(const ArrayType &rhs) const;
  bool operator!=(const ArrayType &rhs) const;

  atl::shared_ptr<ArrayType> getptr() { return shared_from_this(); }

  atl::string astClass() const override { return "ArrayType"; }

  VISITOR_ACCEPTORS
};

} // namespace ACC
