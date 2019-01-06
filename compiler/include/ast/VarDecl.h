#pragma once

#ifndef VARDECL_H
#define VARDECL_H

#include "Decl.h"
#include "Stmt.h"
#include "Type.h"

namespace ACC {

class FunDecl;

class VarDecl : public Decl,
                public Stmt,
                public std::enable_shared_from_this<VarDecl> {

public:
  bool isExtern;
  std::shared_ptr<Type> type;
  std::string identifer;
  std::shared_ptr<FunDecl> parentFunc;

  int fpOffset = 0;

  VarDecl(std::shared_ptr<Type> type, std::string p_identifer,
          bool isExtern = false);

  std::shared_ptr<VarDecl::Decl> getptr();

  bool operator==(Decl &rhs) const override;
  bool operator!=(Decl &rhs) const override;

  bool operator==(const VarDecl &rhs) const;
  bool operator!=(const VarDecl &rhs) const;

  int getBytes() const;

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