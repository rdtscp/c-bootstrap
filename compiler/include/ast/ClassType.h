#pragma once

#ifndef ACC_CLASSTYPE_H
#define ACC_CLASSTYPE_H

#include "ClassTypeDecl.h"

#include "Type.h"

namespace ACC {

class ClassType : public Type, public atl::enable_shared_from_this<ClassType> {

public:
  atl::shared_ptr<Identifier> identifier;
  atl::shared_ptr<ClassTypeDecl> typeDefinition;

  ClassType(const atl::shared_ptr<Identifier> &identifier);

  atl::shared_ptr<ClassType> getptr();

  int getBytes() const override;

  bool operator==(Type &rhs) const override;
  bool operator!=(Type &rhs) const override;

  bool operator==(const ClassType &rhs) const;
  bool operator!=(const ClassType &rhs) const;

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

#endif