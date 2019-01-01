#pragma once

#ifndef FUNDEF_H
#define FUNDEF_H

#include <vector>

#include "Block.h"
#include "Decl.h"
#include "Type.h"
#include "VarDecl.h"

namespace ACC {

class FunDef : public Decl, public std::enable_shared_from_this<FunDef> {

public:
  bool isExtern;
  std::shared_ptr<Block> funBlock;
  std::string funName;
  std::vector<std::shared_ptr<VarDecl>> funParams;
  std::shared_ptr<Type> funType;

  FunDef(std::shared_ptr<Block> funBlock, std::string funName,
         std::vector<std::shared_ptr<VarDecl>> funParams,
         std::shared_ptr<Type> funType, bool isExtern = false);

  std::shared_ptr<FunDef> getptr();

  bool operator==(Decl &rhs) const override;
  bool operator!=(Decl &rhs) const override;

  bool operator==(const FunDef &rhs) const;
  bool operator!=(const FunDef &rhs) const;

  std::string getIdentifier() const override;

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