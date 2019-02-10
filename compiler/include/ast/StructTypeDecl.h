#pragma once

#ifndef ACC_STRUCTTYPEDECL_H
#define ACC_STRUCTTYPEDECL_H

#include "Decl.h"
#include "StructType.h"
#include "VarDecl.h"

namespace ACC {

class StructTypeDecl : public Decl,
                       public atl::enable_shared_from_this<StructTypeDecl> {

public:
  atl::shared_ptr<StructType> structType;
  atl::vector<atl::shared_ptr<VarDecl>> varDecls;
  int allocSize;
  int compactSize;

  StructTypeDecl(atl::shared_ptr<StructType> structType,
                 const atl::vector<atl::shared_ptr<VarDecl>> &varDecls);

  atl::shared_ptr<StructTypeDecl> getptr();

  bool operator==(Decl &rhs) const override;
  bool operator!=(Decl &rhs) const override;

  bool operator==(const StructTypeDecl &rhs) const;
  bool operator!=(const StructTypeDecl &rhs) const;

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