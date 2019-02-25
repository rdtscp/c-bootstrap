#pragma once

#ifndef ACC_STRUCTTYPEDECL_H
#define ACC_STRUCTTYPEDECL_H

#include "ClassType.h"
#include "Decl.h"
#include "FunDecl.h"
#include "VarDecl.h"

namespace ACC {

class ClassTypeDecl : public Decl,
                      public atl::enable_shared_from_this<ClassTypeDecl> {

public:
  atl::shared_ptr<StructType> structType;
  atl::vector<atl::shared_ptr<VarDecl>> varDecls;
  atl::vector<atl::shared_ptr<FunDecl>> funDecls;
  int allocSize;
  int compactSize;

  ClassTypeDecl(atl::shared_ptr<ClassType> structType,
                const atl::vector<atl::shared_ptr<VarDecl>> &varDecls,
                const atl::vector<atl::shared_ptr<FunDecl>> &funDecls);

  atl::shared_ptr<ClassTypeDecl> getptr();

  bool operator==(Decl &rhs) const override;
  bool operator!=(Decl &rhs) const override;

  bool operator==(const ClassTypeDecl &rhs) const;
  bool operator!=(const ClassTypeDecl &rhs) const;

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