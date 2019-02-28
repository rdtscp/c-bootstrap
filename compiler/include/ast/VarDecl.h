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
                public atl::enable_shared_from_this<VarDecl> {

public:
  atl::shared_ptr<Type> type;
  atl::string identifer;
  atl::shared_ptr<FunDecl> parentFunc;

  int fpOffset = 0;

  VarDecl(atl::shared_ptr<Type> type, const atl::string &identifer);

  atl::shared_ptr<VarDecl> getptr();

  bool operator==(Decl &rhs) const override;
  bool operator!=(Decl &rhs) const override;

  bool operator==(const VarDecl &rhs) const;
  bool operator!=(const VarDecl &rhs) const;

  int getBytes() const;

  atl::string getIdentifier() const override;

  void accept(ASTVisitor<void> &v) override;
  atl::string accept(ASTVisitor<atl::string> &v) override;
  atl::shared_ptr<Type> accept(ASTVisitor<atl::shared_ptr<Type>> &v) override;
  MIPS::Register accept(ASTVisitor<MIPS::Register> &v) override;
  atl::shared_ptr<X86::Operand>
  accept(ASTVisitor<atl::shared_ptr<X86::Operand>> &v) override;
  atl::shared_ptr<ASTNode>
  accept(ASTVisitor<atl::shared_ptr<ASTNode>> &v) override;
  atl::string astClass() const override;
};

}; // namespace ACC

#endif