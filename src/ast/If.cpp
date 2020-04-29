#include "ast/If.h"

using namespace ACC;

If::If(const atl::shared_ptr<Expr> &p_ifCondition,
       const atl::shared_ptr<Stmt> &p_ifBody)
    : ifCondition(p_ifCondition), ifBody(p_ifBody), elseBody(nullptr) {}

If::If(const atl::shared_ptr<Expr> &p_ifCondition,
       const atl::shared_ptr<Stmt> &p_ifBody,
       const atl::shared_ptr<Stmt> &p_elseBody)
    : ifCondition(p_ifCondition), ifBody(p_ifBody), elseBody(p_elseBody) {}

bool If::operator==(const If &rhs) const {
  if (*ifCondition != *rhs.ifCondition)
    return false;

  if (*ifBody != *rhs.ifBody)
    return false;

  return true;
}

bool If::operator!=(const If &rhs) const { return !(*this == rhs); }

bool If::operator==(const Stmt &rhs) const {
  if (rhs.astClass() == astClass())
    return *this == *static_cast<const If *>(&rhs);
  return false;
}

bool If::operator!=(const Stmt &rhs) const { return !(*this == rhs); }