#include "ast/Assign.h"

using namespace ACC;

Assign::Assign(const atl::shared_ptr<Expr> &p_lhs,
               const atl::shared_ptr<Expr> &p_rhs)
    : lhs(p_lhs), rhs(p_rhs) {}
