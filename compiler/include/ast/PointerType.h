#pragma once

#include "PrimitiveType.h"
#include "Type.h"

namespace ACC {

class PointerType : public Type,
                    public atl::enable_shared_from_this<PointerType> {

public:
  atl::shared_ptr<Type> pointedType;

  PointerType(const atl::shared_ptr<Type> &p_pointedType);

  int getBytes() const override;

  atl::string getSignature() const;

  bool operator==(Type &rhs) const override;
  bool operator!=(Type &rhs) const override;

  bool operator==(const PointerType &rhs) const;
  bool operator!=(const PointerType &rhs) const;

  atl::shared_ptr<PointerType> getptr() { return shared_from_this(); }

  atl::string astClass() const override { return "PointerType"; }

  VISITOR_ACCEPTORS
};

} // namespace ACC
