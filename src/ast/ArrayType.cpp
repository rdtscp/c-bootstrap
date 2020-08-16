#include "ast/ArrayType.h"
#include "ast/IntLiteral.h"
#include "ast/PointerType.h"

#include "Error.h"

using namespace ACC;

ArrayType::ArrayType(const atl::shared_ptr<Type> &p_type,
                     const atl::shared_ptr<Expr> &p_size)
    : PointerType(p_type), size(p_size) {}

unsigned int ArrayType::getBytes() const {
  unsigned int elementSize = pointedType->getBytes();
  if (size->astClass() != "IntLiteral") {
    return 8;
  }
  // throw ACC::Error(
  //     "Internal Error: Attempted to getBytes() of dynamic ArrayType.");

  const atl::shared_ptr<IntLiteral> sizeIntLiteral =
      atl::static_pointer_cast<IntLiteral>(size);
  return atl::stoi(sizeIntLiteral->getLiteral()) * elementSize;
}

bool ArrayType::canCastTo(Type &rhs) const {
  // TODO Handle Polymorphism.
  if (rhs.astClass() == "PointerType") {
    const PointerType &at = *static_cast<PointerType *>(&rhs);
    return pointedType->equivalentTo(*at.pointedType);
  }

  return (*this == rhs);
}

bool ArrayType::equivalentTo(Type &rhs) const {
  if (rhs.astClass() == "PointerType") {
    const PointerType &pt = *static_cast<PointerType *>(&rhs);
    return *pointedType == *pt.pointedType;
  }
  return *this == rhs;
}

bool ArrayType::operator==(const ArrayType &rhs) const {
  return (*pointedType == *rhs.pointedType && *size == *rhs.size);
}

bool ArrayType::operator!=(const ArrayType &rhs) const {
  return !(*this == rhs);
}

bool ArrayType::operator==(Type &rhs) const {
  if (rhs.astClass() == astClass())
    return *this == *static_cast<ArrayType *>(&rhs);
  return false;
}

bool ArrayType::operator!=(Type &t) const { return !(*this == t); }
