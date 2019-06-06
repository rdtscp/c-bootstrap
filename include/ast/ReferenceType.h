#pragma once

#include "PrimitiveType.h"
#include "Type.h"

namespace ACC {

class ReferenceType : public Type,
                      public atl::enable_shared_from_this<ReferenceType> {

public:
  atl::shared_ptr<Type> referencedType;

  ReferenceType(const atl::shared_ptr<Type> &p_referencedType);

  virtual bool canCastTo(Type &rhs) const override;
  unsigned int getBytes() const override;
  virtual bool equivalentTo(Type &rhs) const override;

  bool operator==(Type &rhs) const override;
  bool operator!=(Type &rhs) const override;

  bool operator==(const ReferenceType &rhs) const;
  bool operator!=(const ReferenceType &rhs) const;

  atl::shared_ptr<ReferenceType> getptr() { return shared_from_this(); }

  atl::string astClass() const override { return "ReferenceType"; }

  VISITOR_ACCEPTORS
};

} // namespace ACC
