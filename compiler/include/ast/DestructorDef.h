
#pragma once

#ifndef ACC_DESTRUCTORDEF_H
#define ACC_DESTRUCTORDEF_H

#include "Block.h"
#include "DestructorDecl.h"

namespace ACC {

class DestructorDef : public DestructorDecl {

public:
  atl::shared_ptr<Block> destructorBlock;

  DestructorDef(const atl::shared_ptr<Type> &p_classType,
                const atl::shared_ptr<Block> &p_destructorBlock);

  atl::shared_ptr<DestructorDecl> getptr();

  bool operator==(Decl &rhs) const override;
  bool operator!=(Decl &rhs) const override;

  bool operator==(const DestructorDef &rhs) const;
  bool operator!=(const DestructorDef &rhs) const;

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

#endif