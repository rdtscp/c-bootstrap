#include "ast/While.h"

using namespace ACC;

While::While(const atl::shared_ptr<Stmt> &p_body,
             const atl::shared_ptr<Expr> &p_condition)
    : body(p_body), condition(p_condition) {}

bool While::operator==(const Stmt &rhs) const {
  if (rhs.astClass() == astClass())
    return *this == *static_cast<const While *>(&rhs);
  return false;
}

bool While::operator!=(const Stmt &rhs) const { return !(*this == rhs); }

bool While::operator==(const While &rhs) const {
  if (*body != *rhs.body)
    return false;

  if (*condition != *rhs.condition)
    return false;

  return true;
}

bool While::operator!=(const While &rhs) const { return !(*this == rhs); }
