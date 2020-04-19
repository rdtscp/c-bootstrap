#pragma once

#include "Expr.h"
#include "PointerType.h"

namespace ACC {

class ArrayType : public PointerType {

public:
  atl::shared_ptr<Expr> size;

  ArrayType(const atl::shared_ptr<Type> &p_type,
            const atl::shared_ptr<Expr> &p_size);

  unsigned int getBytes() const override;
  virtual bool equivalentTo(Type &rhs) const override;

  bool operator==(const ArrayType &rhs) const;
  bool operator!=(const ArrayType &rhs) const;

  bool operator==(Type &rhs) const override;
  bool operator!=(Type &rhs) const override;

  atl::shared_ptr<PointerType> getptr() {
    return PointerType::shared_from_this();
  }

  atl::string astClass() const override { return "ArrayType"; }

  VISITOR_ACCEPTORS
};

} // namespace ACC
