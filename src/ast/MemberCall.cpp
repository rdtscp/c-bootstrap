#include "ast/MemberCall.h"

using namespace ACC;

MemberCall::MemberCall(const atl::shared_ptr<Expr> &p_object,
                       const atl::shared_ptr<FunCall> &p_funCall,
                       const SourceToken::Class &p_accessType)
    : accessType(p_accessType), object(p_object), funCall(p_funCall) {}

bool MemberCall::operator==(Expr &rhs) const {
  if (rhs.astClass() == astClass())
    return *this == *static_cast<MemberCall *>(&rhs);
  return false;
}

bool MemberCall::operator!=(Expr &rhs) const { return !(*this == rhs); }

bool MemberCall::operator==(const MemberCall &rhs) const {
  if (*object != *rhs.object)
    return false;

  if (*funCall != *rhs.funCall)
    return false;

  return true;
}

bool MemberCall::operator!=(const MemberCall &rhs) const { return !(*this == rhs); }

bool MemberCall::operator==(const Stmt &rhs) const {
  if (rhs.astClass() == astClass())
    return *this == *static_cast<const MemberCall *>(&rhs);
  return false;
}

bool MemberCall::operator!=(const Stmt &rhs) const { return !(*this == rhs); }
