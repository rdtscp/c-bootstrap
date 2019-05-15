#include "ast/ReferenceType.h"

using namespace ACC;

ReferenceType::ReferenceType(const atl::shared_ptr<Type> &p_referencedType)
    : referencedType(p_referencedType) {}

unsigned int ReferenceType::getBytes() const { return 4; }

bool ReferenceType::operator==(Type &rhs) const {
  if (referencedType->astClass() == rhs.astClass()) {
    return *referencedType == rhs;
  } else {
    if (rhs.astClass() == astClass())
      return *this == *static_cast<ReferenceType *>(&rhs);
    return false;
  }
}

bool ReferenceType::operator!=(Type &t) const { return !(*this == t); }

bool ReferenceType::operator==(const ReferenceType &rhs) const {
  return *referencedType == *rhs.referencedType;
}

bool ReferenceType::operator!=(const ReferenceType &rhs) const {
  return !(*this == rhs);
}
