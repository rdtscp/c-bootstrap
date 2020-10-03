#include "ast/ReferenceType.h"

using namespace ACC;

ReferenceType::ReferenceType(const atl::shared_ptr<Type> &p_referencedType)
    : referencedType(p_referencedType) {}

bool ReferenceType::canCastTo(Type &rhs) const {
  if (*this == rhs)
    return true;

  return this->referencedType->canCastTo(rhs);
}

unsigned int ReferenceType::getBytes() const {
  return referencedType->getBytes();
}
bool ReferenceType::equivalentTo(Type &rhs) const {
  return referencedType->equivalentTo(rhs);
}

atl::string ReferenceType::mangle() const {
  atl::string output = referencedType->mangle() + "_ref";
  if (typeModifiers.find(Type::Modifiers::CONST))
    output += "_const";
  return output;
}

bool ReferenceType::operator==(Type &rhs) const {
  if (rhs.astClass() == astClass())
    return *this == *static_cast<ReferenceType *>(&rhs);
  return false;
}

bool ReferenceType::operator!=(Type &t) const { return !(*this == t); }

bool ReferenceType::operator==(const ReferenceType &rhs) const {
  return *referencedType == *rhs.referencedType;
}

bool ReferenceType::operator!=(const ReferenceType &rhs) const {
  return !(*this == rhs);
}
