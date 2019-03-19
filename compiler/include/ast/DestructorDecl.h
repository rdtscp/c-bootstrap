#pragma once

#ifndef ACC_DESTRUCTORDECL_H
#define ACC_DESTRUCTORDECL_H

#include "ClassType.h"
// #include "Decl.h "
#include "VarDecl.h"

namespace ACC {

class DestructorDecl : public Decl,
                       public atl::enable_shared_from_this<DestructorDecl> {

public:
  atl::shared_ptr<ClassType> classType;

  DestructorDecl(const atl::shared_ptr<Type> &classType);

  DestructorDecl(const DestructorDecl &rhs) = delete;
  DestructorDecl &operator=(const DestructorDecl &rhs) = delete;
  virtual ~DestructorDecl() {}

  atl::shared_ptr<DestructorDecl> getptr();

  bool operator==(Decl &rhs) const override;
  bool operator!=(Decl &rhs) const override;

  bool operator==(const DestructorDecl &rhs) const;
  bool operator!=(const DestructorDecl &rhs) const;

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