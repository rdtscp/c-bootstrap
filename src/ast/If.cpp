#include "ast/If.h"

using namespace ACC;

If::If(const atl::shared_ptr<Expr> &p_ifCondition,
       const atl::shared_ptr<Stmt> &p_ifBody)
    : ifCondition(p_ifCondition), ifBody(p_ifBody), elseBody(nullptr) {}

If::If(const atl::shared_ptr<Expr> &p_ifCondition,
       const atl::shared_ptr<Stmt> &p_ifBody,
       const atl::shared_ptr<Stmt> &p_elseBody)
    : ifCondition(p_ifCondition), ifBody(p_ifBody), elseBody(p_elseBody) {}
