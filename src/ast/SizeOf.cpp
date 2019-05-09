#include "ast/SizeOf.h"

using namespace ACC;

SizeOf::SizeOf(const atl::shared_ptr<Type> &p_type) : type(p_type) {}

atl::string SizeOf::getSignature() const { return "int"; }

bool SizeOf::operator==(Expr &rhs) const {
  if (rhs.astClass() == astClass())
    return *this == *static_cast<SizeOf *>(&rhs);
  return false;
}

bool SizeOf::operator!=(Expr &rhs) const { return !(*this == rhs); }

bool SizeOf::operator==(const SizeOf &rhs) const {
  if (*type != *rhs.type)
    return false;

  return true;
}

bool SizeOf::operator!=(const SizeOf &rhs) const { return !(*this == rhs); }
