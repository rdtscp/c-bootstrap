#include "ast/MemberAccess.h"

using namespace ACC;

MemberAccess::MemberAccess(const atl::shared_ptr<Expr> &p_object,
                           const atl::shared_ptr<VarExpr> &p_fieldVariable,
                           const SourceToken::Class &p_accessType)
    : accessType(p_accessType), object(p_object),
      fieldVariable(p_fieldVariable) {}

bool MemberAccess::operator==(Expr &rhs) const {
  if (rhs.astClass() == astClass())
    return *this == *static_cast<MemberAccess *>(&rhs);
  return false;
}

bool MemberAccess::operator!=(Expr &rhs) const { return !(*this == rhs); }

bool MemberAccess::operator==(const MemberAccess &rhs) const {
  if (*object != *rhs.object)
    return false;

  if (*fieldVariable != *rhs.fieldVariable)
    return false;

  return true;
}

bool MemberAccess::operator!=(const MemberAccess &rhs) const {
  return !(*this == rhs);
}
