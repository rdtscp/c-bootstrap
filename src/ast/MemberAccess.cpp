#include "ast/MemberAccess.h"

using namespace ACC;

MemberAccess::MemberAccess(const atl::shared_ptr<Expr> &p_object,
                           const atl::shared_ptr<Identifier> &p_fieldIdentifier)
    : object(p_object), fieldIdentifier(p_fieldIdentifier) {}

bool MemberAccess::operator==(Expr &rhs) const {
  if (rhs.astClass() == astClass())
    return *this == *static_cast<MemberAccess *>(&rhs);
  return false;
}

bool MemberAccess::operator!=(Expr &rhs) const { return !(*this == rhs); }

bool MemberAccess::operator==(const MemberAccess &rhs) const {
  if (*object != *rhs.object)
    return false;

  if (*fieldIdentifier != *rhs.fieldIdentifier)
    return false;

  return true;
}

bool MemberAccess::operator!=(const MemberAccess &rhs) const {
  return !(*this == rhs);
}
