#pragma once

#ifndef ACC_VARDEF_H
#define ACC_VARDEF_H

#include "Decl.h"
#include "Literal.h"
#include "Stmt.h"
#include "Type.h"
#include "VarDecl.h"

namespace ACC {

class VarDef : public VarDecl {

public:
  const atl::shared_ptr<Literal> value;

  int fpOffset = 0;

  VarDef(const atl::shared_ptr<Type> &type, const atl::string &identifer,
         const atl::shared_ptr<Literal> &value, bool isExtern = false);

  atl::shared_ptr<VarDecl> getptr();

  bool operator==(Decl &rhs) const override;
  bool operator!=(Decl &rhs) const override;

  bool operator==(const VarDef &rhs) const;
  bool operator!=(const VarDef &rhs) const;

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