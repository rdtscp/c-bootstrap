#pragma once

#ifndef BLOCK_H
#define BLOCK_H

#include <map>
#include <vector>

#include "Decl.h"
#include "Stmt.h"

namespace ACC {

class Block : public Stmt {

public:
  std::map<std::string, std::shared_ptr<Decl>> blockDecls;
  std::vector<std::shared_ptr<Stmt>> blockStmts;
  std::shared_ptr<Block> outerBlock = nullptr;

  Block(const std::vector<std::shared_ptr<Stmt>> &newBlockStmts)
      : blockStmts(newBlockStmts) {}

  std::shared_ptr<Decl> find(const std::string &identifier) {
    std::shared_ptr<Decl> local = findCurrent(identifier);
    if (local == nullptr && outerBlock != nullptr)
      return outerBlock->find(identifier);

    return nullptr;
  }

  std::shared_ptr<Decl> findCurrent(const std::string &identifier) {
    if (blockDecls.find(identifier) != blockDecls.end())
      return blockDecls.find(identifier)->second;

    return nullptr;
  }

  void insertDecl(const std::shared_ptr<Decl> &decl) {
    blockDecls[decl->getIdentifier()] = decl;
  }

  void setOuterBlock(const std::shared_ptr<Block> &newOuterBlock) {
    outerBlock = newOuterBlock;
  }

  std::string accept(ASTVisitor &v) override { return v.visit(*this); }
  std::string astClass() const override { return "Block"; }
};

}; // namespace ACC

#endif