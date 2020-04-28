#pragma once

#include "PrimitiveType.h"
#include "Type.h"

namespace ACC {

class ReferenceType : public Type,
                      public atl::enable_shared_from_this<ReferenceType> {

public:
  atl::shared_ptr<Type> referencedType;

  ReferenceType(const atl::shared_ptr<Type> &p_referencedType);

  bool canCastTo(Type &rhs) const override;
  unsigned int getBytes() const override;
  bool equivalentTo(Type &rhs) const override;

  bool operator==(Type &rhs) const override;
  bool operator!=(Type &rhs) const override;

  bool operator==(const ReferenceType &rhs) const;
  bool operator!=(const ReferenceType &rhs) const;

  atl::shared_ptr<ReferenceType> getptr() { return shared_from_this(); }

  atl::string astClass() const override { return "ReferenceType"; }

  static atl::shared_ptr<Type> collapseReferenceTypes(atl::shared_ptr<Type> type) {
    if (type->astClass() == "ReferenceType") {
      type = atl::static_pointer_cast<ReferenceType>(type)->referencedType;
      if (type->astClass() == "ReferenceType")
        type = atl::static_pointer_cast<ReferenceType>(type)->referencedType;
    }
    return type;
  }

  VISITOR_ACCEPTORS
};

} // namespace ACC
