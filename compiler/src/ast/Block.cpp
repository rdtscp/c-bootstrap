#include "../../include/ast/Block.h"

using namespace ACC;

Block::Block(const std::vector<std::shared_ptr<Stmt>> &newBlockStmts)
    : blockStmts(newBlockStmts) {}

std::shared_ptr<Block> Block::getptr() { return shared_from_this(); }

std::shared_ptr<Decl> Block::find(const atl::string &identifier) {
  std::shared_ptr<Decl> local = findLocal(identifier);
  if (local == nullptr && outerBlock != nullptr)
    return outerBlock->find(identifier);

  return local;
}
bool Block::operator==(const Block &rhs) const {
  if (blockStmts.size() != rhs.blockStmts.size())
    return false;

  for (int i = 0; i < blockStmts.size(); i++)
    /* @TODO Implement comparitors for Stmts. */
    if (blockStmts[i]->astClass() != rhs.blockStmts[i]->astClass())
      return false;

  return true;
}

bool Block::operator!=(const Block &rhs) const { return !(*this == rhs); }

std::shared_ptr<Decl> Block::findLocal(const atl::string &identifier) {
  std::string ident = identifier.c_str();
  if (blockDecls.find(ident) != blockDecls.end())
    return blockDecls.find(ident)->second;

  return nullptr;
}

void Block::insertDecl(const std::shared_ptr<Decl> &decl) {
  blockDecls[decl->getIdentifier().c_str()] = decl;
}

void Block::setOuterBlock(const std::shared_ptr<Block> &newOuterBlock) {
  outerBlock = newOuterBlock;
}

void Block::accept(ASTVisitor<void> &v) { return v.visit(*this); }

atl::string Block::accept(ASTVisitor<atl::string> &v) { return v.visit(*this); }

std::shared_ptr<Type> Block::accept(ASTVisitor<std::shared_ptr<Type>> &v) {
  return v.visit(*this);
}

MIPS::Register Block::accept(ASTVisitor<MIPS::Register> &v) {
  return v.visit(*this);
}

std::shared_ptr<X86::Operand>
Block::accept(ASTVisitor<std::shared_ptr<X86::Operand>> &v) {
  return v.visit(*this);
}

std::shared_ptr<ASTNode>
Block::accept(ASTVisitor<std::shared_ptr<ASTNode>> &v) {
  return v.visit(*this);
}

atl::string Block::astClass() const { return "Block"; }