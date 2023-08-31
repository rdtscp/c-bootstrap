#include "ast/DoWhile.h"

using namespace ACC;

DoWhile::DoWhile(const atl::shared_ptr<Stmt> &p_body,
                 const atl::shared_ptr<Expr> &p_condition)
    : body(p_body), condition(p_condition) {}

bool DoWhile::operator==(const DoWhile &rhs) const {
  if (*body != *rhs.body)
    return false;

  if (*condition != *rhs.condition)
    return false;

  return true;
}

bool DoWhile::operator!=(const DoWhile &rhs) const { return !(*this == rhs); }

bool DoWhile::operator==(const Stmt &rhs) const {
  if (rhs.astClass() == astClass())
    return *this == *static_cast<const DoWhile *>(&rhs);
  return false;
}

bool DoWhile::operator!=(const Stmt &rhs) const { return !(*this == rhs); }