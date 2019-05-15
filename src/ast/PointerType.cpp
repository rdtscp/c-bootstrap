#include "ast/PointerType.h"
#include "ast/ArrayType.h"
#include "ast/ReferenceType.h"

using namespace ACC;

PointerType::PointerType(const atl::shared_ptr<Type> &p_pointedType)
    : pointedType(p_pointedType) {}

unsigned int PointerType::getBytes() const { return 4u; }

bool PointerType::operator==(Type &rhs) const {
  if (rhs.astClass() == "ArrayType") {
    const ArrayType &at = *static_cast<ArrayType *>(&rhs);
    return pointedType->astClass() == at.pointedType->astClass();
  } else if (rhs.astClass() == "ReferenceType") {
    const atl::shared_ptr<ReferenceType> rhsRefType(
        static_cast<ReferenceType *>(&rhs));
    return *this == *rhsRefType->referencedType;
  } else {
    if (rhs.astClass() == astClass())
      return *this == *static_cast<PointerType *>(&rhs);
    return false;
  }
}

bool PointerType::operator!=(Type &t) const { return !(*this == t); }

bool PointerType::operator==(const PointerType &rhs) const {
  return *pointedType == *rhs.pointedType;
}

bool PointerType::operator!=(const PointerType &rhs) const {
  return !(*this == rhs);
}
