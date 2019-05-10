#include "ast/BaseType.h"
#include "ast/ReferenceType.h"

using namespace ACC;

BaseType::BaseType(PrimitiveType p_pType) : primitiveType(p_pType) {}

int BaseType::getBytes() const {
  switch (primitiveType) {
  case PrimitiveType::CHAR:
    return 1;
  case PrimitiveType::INT:
    return 4;
  case PrimitiveType::SHORT:
    return 4;
  case PrimitiveType::VOID:
    return 4;
  case PrimitiveType::UINT:
    return 4;
  case PrimitiveType::BOOL:
    return 1;
  case PrimitiveType::NULLPTR_T:
    return 4;
  default:
    return 4;
  }
}

atl::string BaseType::getSignature() const {
  switch (primitiveType) {
  case PrimitiveType::CHAR:
    return "char";
  case PrimitiveType::INT:
    return "int";
  case PrimitiveType::SHORT:
    return "short";
  case PrimitiveType::VOID:
    return "void";
  case PrimitiveType::UINT:
    return "uint";
  case PrimitiveType::BOOL:
    return "bool";
  case PrimitiveType::NULLPTR_T:
    return "nullptr_t";
  default:
    return "nullptr_t";
  }
}

bool BaseType::operator==(Type &rhs) const {
  if (rhs.astClass() == "ReferenceType") {
    const atl::shared_ptr<ReferenceType> rhsRefType(
        static_cast<ReferenceType *>(&rhs));
    return *this == *rhsRefType->referencedType;
  } else {
    if (rhs.astClass() == astClass())
      return *this == *static_cast<BaseType *>(&rhs);
    return false;
  }
}
bool BaseType::operator!=(Type &t) const { return !(*this == t); }

bool BaseType::operator==(const BaseType &rhs) const {
  return primitiveType == rhs.primitiveType;
}
bool BaseType::operator!=(const BaseType &rhs) const { return !(*this == rhs); }
