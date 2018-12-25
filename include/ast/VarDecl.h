#pragma once

#ifndef VARDECL_H
#define VARDECL_H

#include <memory>

#include "Decl.h"
#include "Stmt.h"
#include "Type.h"

namespace ACC {

class FunDecl;

class VarDecl : public Decl,
                public Stmt,
                public std::enable_shared_from_this<VarDecl> {

public:
  std::shared_ptr<Type> type;
  std::string identifer;
  std::shared_ptr<FunDecl> parentFunc;

  int fpOffset = 0;

  VarDecl(std::shared_ptr<Type> type, std::string p_identifer);

  std::shared_ptr<VarDecl> getptr();

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
  X86::Register accept(ASTVisitor<X86::Register> &v) override;
  std::string astClass() const override;
};

}; // namespace ACC

#endif