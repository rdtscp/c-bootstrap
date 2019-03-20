#pragma once

#include "ClassType.h"
#include "Decl.h"
#include "FunDecl.h"
#include "VarDecl.h"

namespace ACC {

class ClassTypeDecl : public Decl,
                      public atl::enable_shared_from_this<ClassTypeDecl> {

public:
  atl::shared_ptr<ClassType> classType;
  atl::vector<atl::shared_ptr<Decl>> classDecls;

  ClassTypeDecl(const atl::shared_ptr<ClassType> &p_classType,
                const atl::vector<atl::shared_ptr<Decl>> &p_classDecls);

  atl::shared_ptr<ClassTypeDecl> getptr();

  bool operator==(Decl &rhs) const override;
  bool operator!=(Decl &rhs) const override;

  bool operator==(const ClassTypeDecl &rhs) const;
  bool operator!=(const ClassTypeDecl &rhs) const;

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

} // namespace ACC
