#include "../../include/ast/Block.h"

using namespace ACC;

Block::Block(const atl::vector<atl::shared_ptr<Stmt>> &p_stmts)
    : stmts(p_stmts) {}

atl::shared_ptr<Block> Block::getptr() { return shared_from_this(); }

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

void Block::accept(ASTVisitor<void> &v) { return v.visit(*this); }

atl::string Block::accept(ASTVisitor<atl::string> &v) { return v.visit(*this); }

atl::shared_ptr<Type> Block::accept(ASTVisitor<atl::shared_ptr<Type>> &v) {
  return v.visit(*this);
}

MIPS::Register Block::accept(ASTVisitor<MIPS::Register> &v) {
  return v.visit(*this);
}

atl::shared_ptr<X86::Operand>
Block::accept(ASTVisitor<atl::shared_ptr<X86::Operand>> &v) {
  return v.visit(*this);
}

atl::shared_ptr<ASTNode>
Block::accept(ASTVisitor<atl::shared_ptr<ASTNode>> &v) {
  return v.visit(*this);
}

atl::string Block::astClass() const { return "Block"; }