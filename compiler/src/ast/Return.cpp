#include "ast/Return.h"

using namespace ACC;

Return::Return() : returnExpr(nullptr) {}

Return::Return(const atl::shared_ptr<Expr> &p_returnExpr)
    : returnExpr(p_returnExpr) {}
