#include "ast/TertiaryExpr.h"

using namespace ACC;

TertiaryExpr::TertiaryExpr(const atl::shared_ptr<Expr> &p_tertiaryCondition,
                           const atl::shared_ptr<Expr> &p_tertiaryIfBody,
                           const atl::shared_ptr<Expr> &p_tertiaryElseBody)
    : tertiaryCondition(p_tertiaryCondition), tertiaryIfBody(p_tertiaryIfBody),
      tertiaryElseBody(p_tertiaryElseBody) {}

bool TertiaryExpr::operator==(Expr &rhs) const {
  if (rhs.astClass() == astClass())
    return *this == *static_cast<TertiaryExpr *>(&rhs);
  return false;
}

bool TertiaryExpr::operator!=(Expr &rhs) const { return !(*this == rhs); }

bool TertiaryExpr::operator==(const Stmt &rhs) const {
  if (rhs.astClass() == astClass())
    return *this == *static_cast<const TertiaryExpr *>(&rhs);
  return false;
}

bool TertiaryExpr::operator!=(const Stmt &rhs) const { return !(*this == rhs); }

bool TertiaryExpr::operator==(const TertiaryExpr &rhs) const {
  if (*tertiaryCondition != *rhs.tertiaryCondition)
    return false;

  if (*tertiaryIfBody != *rhs.tertiaryIfBody)
    return false;

  if (*tertiaryElseBody != *rhs.tertiaryElseBody)
    return false;

  return true;
}

bool TertiaryExpr::operator!=(const TertiaryExpr &rhs) const {
  return !(*this == rhs);
}
