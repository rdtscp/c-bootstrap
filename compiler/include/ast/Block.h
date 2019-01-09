#pragma once

#ifndef ACC_BLOCK_H
#define ACC_BLOCK_H

#include <map>
#include <vector>

#include "Decl.h"
#include "Stmt.h"

namespace ACC {

class Block : public Stmt, public std::enable_shared_from_this<Block> {

public:
  std::map<std::string, std::shared_ptr<Decl>> blockDecls;
  std::vector<std::shared_ptr<Stmt>> blockStmts;
  std::shared_ptr<Block> outerBlock = nullptr;

  Block(const std::vector<std::shared_ptr<Stmt>> &newBlockStmts);

  std::shared_ptr<Block> getptr();

  bool operator==(const Block &rhs) const;
  bool operator!=(const Block &rhs) const;

  std::shared_ptr<Decl> find(const std::string &identifier);

  std::shared_ptr<Decl> findLocal(const std::string &identifier);

  void insertDecl(const std::shared_ptr<Decl> &decl);

  void setOuterBlock(const std::shared_ptr<Block> &newOuterBlock);

  void accept(ASTVisitor<void> &v) override;
  std::string accept(ASTVisitor<std::string> &v) override;
  std::shared_ptr<Type> accept(ASTVisitor<std::shared_ptr<Type>> &v) override;
  MIPS::Register accept(ASTVisitor<MIPS::Register> &v) override;
  std::shared_ptr<X86::Operand>
  accept(ASTVisitor<std::shared_ptr<X86::Operand>> &v) override;
  std::shared_ptr<ASTNode>
  accept(ASTVisitor<std::shared_ptr<ASTNode>> &v) override;
  std::string astClass() const override;
};

}; // namespace ACC

#endif