#pragma once

#ifndef FUNDECL_H
#define FUNDECL_H

#include <vector>

#include "Block.h"
#include "Decl.h"
#include "Type.h"
#include "VarDecl.h"

namespace ACC {

class FunDecl : public Decl, public std::enable_shared_from_this<FunDecl> {

public:
  std::shared_ptr<Block> funBlock;
  std::string funName;
  std::vector<std::shared_ptr<VarDecl>> funParams;
  std::shared_ptr<Type> funType;

  FunDecl(std::shared_ptr<Block> funBlock, std::string funName,
          std::vector<std::shared_ptr<VarDecl>> funParams,
          std::shared_ptr<Type> funType);

  std::shared_ptr<FunDecl> getptr();

  bool operator==(Decl &rhs) const override;
  bool operator!=(Decl &rhs) const override;

  bool operator==(const FunDecl &rhs) const;
  bool operator!=(const FunDecl &rhs) const;

  std::string getIdentifier() const override;

  void accept(ASTVisitor<void> &v) override;
  std::string accept(ASTVisitor<std::string> &v) override;
  std::shared_ptr<Type> accept(ASTVisitor<std::shared_ptr<Type>> &v) override;
  MIPS::Register accept(ASTVisitor<MIPS::Register> &v) override;
  X86::Register accept(ASTVisitor<X86::Register> &v) override;
  std::string astClass() const override;
};

}; // namespace ACC

#endif