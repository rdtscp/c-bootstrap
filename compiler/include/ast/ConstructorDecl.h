#pragma once

#ifndef ACC_CONSTRUCTORDECL_H
#define ACC_CONSTRUCTORDECL_H

#include "ClassType.h"
// #include "Decl.h "
#include "VarDecl.h"

namespace ACC {

class ConstructorDecl : public Decl,
                        public atl::enable_shared_from_this<ConstructorDecl> {

public:
  atl::shared_ptr<ClassType> classType;
  atl::vector<atl::shared_ptr<VarDecl>> constructorParams;

  ConstructorDecl(
      const atl::shared_ptr<Type> &classType,
      const atl::vector<atl::shared_ptr<VarDecl>> &constructorParams);

  ConstructorDecl(const ConstructorDecl &rhs) = delete;
  virtual ~ConstructorDecl() {}
  ConstructorDecl &operator=(const ConstructorDecl &rhs) = delete;

  atl::shared_ptr<ConstructorDecl> getptr();

  bool operator==(Decl &rhs) const override;
  bool operator!=(Decl &rhs) const override;

  bool operator==(const ConstructorDecl &rhs) const;
  bool operator!=(const ConstructorDecl &rhs) const;

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