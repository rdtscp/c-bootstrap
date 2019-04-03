#include "ast/ArrayType.h"
#include "ast/IntLiteral.h"

using namespace ACC;

ArrayType::ArrayType(const atl::shared_ptr<Type> &p_type,
                     const atl::shared_ptr<Expr> &p_size)
    : type(p_type), size(p_size) {}

int ArrayType::getBytes() const {
  int elementSize = type->getBytes();
  if (size->astClass() != "IntLiteral")
    throw "Internal Error: Attempted to getBytes() of dynamic "
          "ArrayType.";

  const atl::shared_ptr<IntLiteral> sizeIntLiteral =
      atl::static_pointer_cast<IntLiteral>(size);
  return std::stoi(std::string(sizeIntLiteral->getLiteral().c_str())) *
         elementSize;
}

bool ArrayType::operator==(Type &rhs) const {
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
