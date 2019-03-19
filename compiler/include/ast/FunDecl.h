#pragma once

#ifndef ACC_FUNDECL_H
#define ACC_FUNDECL_H

#include "Decl.h"
#include "Type.h"
#include "VarDecl.h"

namespace ACC {

class FunDecl : public Decl, public atl::enable_shared_from_this<FunDecl> {

public:
  atl::shared_ptr<Identifier> funIdentifier;
  atl::vector<atl::shared_ptr<VarDecl>> funParams;
  atl::shared_ptr<Type> funType;

  FunDecl(const atl::shared_ptr<Identifier> &funIdentifier,
          const atl::vector<atl::shared_ptr<VarDecl>> &funParams,
          const atl::shared_ptr<Type> &funType);

  FunDecl(const FunDecl &rhs) = delete;
  FunDecl &operator=(const FunDecl &rhs) = delete;
  virtual ~FunDecl() {}

  atl::shared_ptr<FunDecl> getptr();

  bool operator==(Decl &rhs) const override;
  bool operator!=(Decl &rhs) const override;

  bool operator==(const FunDecl &rhs) const;
  bool operator!=(const FunDecl &rhs) const;

  atl::shared_ptr<Identifier> getIdentifier() const override;

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