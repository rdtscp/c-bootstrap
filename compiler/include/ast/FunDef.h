#pragma once

#ifndef ACC_FUNDEF_H
#define ACC_FUNDEF_H

#include <vector>

#include "Block.h"
#include "Decl.h"
#include "FunDecl.h"
#include "Type.h"
#include "VarDecl.h"

namespace ACC {

class FunDef : public FunDecl {

public:
  std::shared_ptr<Block> funBlock;

  FunDef(std::shared_ptr<Block> funBlock, std::string funName,
         std::vector<std::shared_ptr<VarDecl>> funParams,
         std::shared_ptr<Type> funType);

  std::shared_ptr<FunDecl> getptr();

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