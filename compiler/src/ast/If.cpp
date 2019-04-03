#include "ast/If.h"

using namespace ACC;

If::If(const atl::shared_ptr<Expr> &p_ifCondition,
       const atl::shared_ptr<Stmt> &p_ifBody)
    : ifBody(p_ifBody), ifCondition(p_ifCondition), elseBody(nullptr) {}

If::If(const atl::shared_ptr<Expr> &p_ifCondition,
       const atl::shared_ptr<Stmt> &p_ifBody,
       const atl::shared_ptr<Stmt> &p_elseBody)
    : ifBody(p_ifBody), ifCondition(p_ifCondition), elseBody(p_elseBody) {}
