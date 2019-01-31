#pragma once

#ifndef ACC_VARDECL_H
#define ACC_VARDECL_H

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
  atl::string identifer;
  std::shared_ptr<FunDecl> parentFunc;

  int fpOffset = 0;

  VarDecl(std::shared_ptr<Type> type, const atl::string &identifer,
          bool isExtern = false);

  std::shared_ptr<VarDecl::Decl> getptr();

  bool operator==(Decl &rhs) const override;
  bool operator!=(Decl &rhs) const override;

  bool operator==(const VarDecl &rhs) const;
  bool operator!=(const VarDecl &rhs) const;

  int getBytes() const;

  atl::string getIdentifier() const override;

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