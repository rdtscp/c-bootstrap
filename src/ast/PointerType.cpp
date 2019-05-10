#include "ast/PointerType.h"
#include "ast/ArrayType.h"

using namespace ACC;

PointerType::PointerType(const atl::shared_ptr<Type> &p_pointedType)
    : pointedType(p_pointedType) {}

int PointerType::getBytes() const { return 4; }

atl::string PointerType::getSignature() const {
  return pointedType->getSignature() + "*";
}

bool PointerType::operator==(Type &rhs) const {
  if (rhs.astClass() == "ArrayType") {
    const ArrayType &at = *static_cast<ArrayType *>(&rhs);
    return pointedType->astClass() == at.type->astClass();
  }
  if (rhs.astClass() == astClass())
    return *this == *static_cast<PointerType *>(&rhs);
  return false;
}

bool PointerType::operator!=(Type &t) const { return !(*this == t); }

bool PointerType::operator==(const PointerType &rhs) const {
  return *pointedType == *rhs.pointedType;
}

bool PointerType::operator!=(const PointerType &rhs) const {
  return !(*this == rhs);
}
