#include "ast/DoWhile.h"

using namespace ACC;

DoWhile::DoWhile(const atl::shared_ptr<Stmt> &p_body,
                 const atl::shared_ptr<Expr> &p_condition)
    : body(p_body), condition(p_condition) {}
