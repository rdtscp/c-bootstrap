#include "../../include/ast/Block.h"

using namespace ACC;

Block::Block(const atl::vector<atl::shared_ptr<Stmt>> &p_stmts)
    : decls({}), stmts(p_stmts), outerBlock(nullptr) {}

atl::shared_ptr<Block> Block::getptr() { return shared_from_this(); }

atl::shared_ptr<Decl>
Block::find(const atl::shared_ptr<Identifier> &identifier) {
  atl::shared_ptr<Decl> local = findLocal(identifier);
  if (local == nullptr && outerBlock != nullptr)
    return outerBlock->find(identifier);

  return local;
}
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

atl::shared_ptr<Decl>
Block::findLocal(const atl::shared_ptr<Identifier> &identifier) {
  // Reverse iterate decls.
  for (int idx = decls.size() - 1; idx >= 0; --idx)
    if (*identifier == *decls[idx]->getIdentifier())
      return decls[idx];

  return nullptr;
}

void Block::insertDecl(const atl::shared_ptr<Decl> &decl) {
  decls.push_back(decl);
}

void Block::setOuterBlock(const atl::shared_ptr<Block> &newOuterBlock) {
  outerBlock = newOuterBlock;
}

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