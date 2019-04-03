#include "ast/Block.h"

using namespace ACC;

Block::Block(const atl::vector<atl::shared_ptr<Stmt>> &p_stmts)
    : stmts(p_stmts) {}

bool Block::operator==(const Block &rhs) const {
  if (stmts.size() != rhs.stmts.size())
    return false;

  for (int i = 0; i < stmts.size(); ++i)
    /* @TODO Implement comparitors for Stmts. */
    if (stmts[i]->astClass() != rhs.stmts[i]->astClass())
      return false;

  return true;
}

bool Block::operator!=(const Block &rhs) const { return !(*this == rhs); }
