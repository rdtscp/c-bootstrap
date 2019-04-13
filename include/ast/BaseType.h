#pragma once

#include "PrimitiveType.h"
#include "Type.h"

namespace ACC {

class BaseType : public Type, public atl::enable_shared_from_this<BaseType> {

public:
  PrimitiveType primitiveType;

  BaseType(const PrimitiveType p_pType);

  int getBytes() const override;

  atl::string getSignature() const;

  bool operator==(Type &rhs) const override;
  bool operator!=(Type &rhs) const override;

  bool operator==(const BaseType &rhs) const;
  bool operator!=(const BaseType &rhs) const;

  atl::shared_ptr<BaseType> getptr() { return shared_from_this(); }

  atl::string astClass() const override { return "BaseType"; }

  VISITOR_ACCEPTORS
};

} // namespace ACC
