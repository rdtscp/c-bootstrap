#pragma once

#ifndef ACC_PROGRAM_H
#define ACC_PROGRAM_H

#include <map>

#include "../ASTNode.h"

#include "FunDecl.h"
#include "Namespace.h"
#include "StructTypeDecl.h"
#include "VarDecl.h"

namespace ACC {

class Program : public ASTNode {

public:
  atl::vector<std::shared_ptr<Decl>> decls;
  atl::vector<std::shared_ptr<FunDecl>> funDecls;
  atl::vector<std::shared_ptr<VarDecl>> globalVars;
  std::shared_ptr<Block> globalScope = nullptr;
  std::map<std::string, Namespace> namespaces;

  Program(const atl::vector<std::shared_ptr<Decl>> &decls);

  void accept(ASTVisitor<void> &v) override;
  atl::string accept(ASTVisitor<atl::string> &v) override;
  std::shared_ptr<Type> accept(ASTVisitor<std::shared_ptr<Type>> &v) override;
  MIPS::Register accept(ASTVisitor<MIPS::Register> &v) override;
  std::shared_ptr<X86::Operand>
  accept(ASTVisitor<std::shared_ptr<X86::Operand>> &v) override;
  std::shared_ptr<ASTNode>
  accept(ASTVisitor<std::shared_ptr<ASTNode>> &v) override;
  atl::string astClass() const override;
};
}; // namespace ACC

#endif