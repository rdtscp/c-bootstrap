#include "ast/Throw.h"

using namespace ACC;

Throw::Throw(const atl::shared_ptr<StringLiteral> &p_exceptionText)
    : exceptionText(p_exceptionText) {}
