#include "ast/While.h"

using namespace ACC;

While::While(const atl::shared_ptr<Stmt> &p_body,
             const atl::shared_ptr<Expr> &p_condition)
    : body(p_body), condition(p_condition) {}
