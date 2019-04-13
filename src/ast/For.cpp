#include "ast/For.h"

using namespace ACC;

For::For(const atl::shared_ptr<VarDecl> &p_initialVarDecl,
         const atl::shared_ptr<Expr> &p_condition,
         const atl::shared_ptr<Expr> &p_endBodyExpr,
         const atl::shared_ptr<Stmt> &p_body)
    : initialVarDecl(p_initialVarDecl), condition(p_condition),
      endBodyExpr(p_endBodyExpr), body(p_body) {}
