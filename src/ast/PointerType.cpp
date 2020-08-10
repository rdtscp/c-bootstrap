#include "ast/PointerType.h"
#include "ast/ArrayType.h"
#include "ast/BaseType.h"
#include "ast/ReferenceType.h"

using namespace ACC;

PointerType::PointerType(const atl::shared_ptr<Type> &p_pointedType)
    : pointedType(p_pointedType) {}

unsigned int PointerType::getBytes() const { return 4; }

bool PointerType::canCastTo(Type &rhs) const {
  // TODO Handle Polymorphism.

  if (*this == rhs)
    return true;

  return false;
}

bool PointerType::equivalentTo(Type &rhs) const {
  if (rhs.astClass() == "ArrayType") {
    const ArrayType &at = *static_cast<ArrayType *>(&rhs);
    return pointedType->equivalentTo(*at.pointedType);
  }
  if (rhs.astClass() == "PointerType") {
    const PointerType &pt = *static_cast<PointerType *>(&rhs);
    return pointedType->equivalentTo(*pt.pointedType);
  }
  if (rhs.astClass() == "BaseType") {
    const BaseType &bt = *static_cast<BaseType *>(&rhs);
    if (bt.primitiveType == PrimitiveType::NULLPTR_T)
      return true;
    else
      return false;
  }
  return *this == rhs;
}

atl::string PointerType::mangle() const {
  atl::string output = pointedType->mangle() + "_ptr";
  if (typeModifiers.find(Type::Modifiers::CONST))
    output += "_const";
  return output;
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
