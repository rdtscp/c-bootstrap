#include "ast/PointerType.h"

using namespace ACC;

PointerType::PointerType(const atl::shared_ptr<Type> &p_pointedType)
    : pointedType(p_pointedType) {}

int PointerType::getBytes() const { return 4; }

atl::string PointerType::getSignature() const {
  return pointedType->getSignature() +  "*";
}

bool PointerType::operator==(Type &rhs) const {
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
