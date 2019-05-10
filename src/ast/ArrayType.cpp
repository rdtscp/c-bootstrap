#include "ast/ArrayType.h"
#include "ast/IntLiteral.h"
#include "ast/PointerType.h"

#include "Error.h"

using namespace ACC;

ArrayType::ArrayType(const atl::shared_ptr<Type> &p_type,
                     const atl::shared_ptr<Expr> &p_size)
    : type(p_type), size(p_size) {}

int ArrayType::getBytes() const {
  int elementSize = type->getBytes();
  if (size->astClass() != "IntLiteral")
    throw ACC::Error(
        "Internal Error: Attempted to getBytes() of dynamic ArrayType.");

  const atl::shared_ptr<IntLiteral> sizeIntLiteral =
      atl::static_pointer_cast<IntLiteral>(size);
  return atl::stoi(sizeIntLiteral->getLiteral()) * elementSize;
}

atl::string ArrayType::getSignature() const {
  return type->getSignature() + "[]";
}

bool ArrayType::operator==(Type &rhs) const {
  if (rhs.astClass() == "PointerType") {
    const PointerType &pt = *static_cast<PointerType *>(&rhs);
    return type->astClass() == pt.pointedType->astClass();
  }
  if (rhs.astClass() == astClass())
    return *this == *static_cast<ArrayType *>(&rhs);
  return false;
}

bool ArrayType::operator!=(Type &t) const { return !(*this == t); }

bool ArrayType::operator==(const ArrayType &rhs) const {
  return (*type == *rhs.type && *size == *rhs.size);
}

bool ArrayType::operator!=(const ArrayType &rhs) const {
  return !(*this == rhs);
}
