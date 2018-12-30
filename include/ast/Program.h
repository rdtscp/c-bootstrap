#pragma once

#ifndef PROGRAM_H
#define PROGRAM_H

#include "../ASTNode.h"

#include "FunDecl.h"
#include "StructTypeDecl.h"
#include "VarDecl.h"

namespace ACC {

class Program : public ASTNode {

public:
  std::vector<std::shared_ptr<Decl>> decls;
  std::vector<std::shared_ptr<FunDecl>> funDecls;
  std::vector<std::shared_ptr<VarDecl>> globalVars;
  std::shared_ptr<Block> globalScope = nullptr;

  Program(const std::vector<std::shared_ptr<Decl>> &decls);

  void accept(ASTVisitor<void> &v) override;
  std::string accept(ASTVisitor<std::string> &v) override;
  std::shared_ptr<Type> accept(ASTVisitor<std::shared_ptr<Type>> &v) override;
  MIPS::Register accept(ASTVisitor<MIPS::Register> &v) override;
  std::shared_ptr<X86::Operand> accept(ASTVisitor<std::shared_ptr<X86::Operand>> &v) override;
  std::string astClass() const override;
};
}; // namespace ACC

#endif