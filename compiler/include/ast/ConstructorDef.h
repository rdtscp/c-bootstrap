
#pragma once

#include "Block.h"
#include "ConstructorDecl.h"

namespace ACC {

class ConstructorDef : public ConstructorDecl {

public:
  atl::vector<atl::shared_ptr<Assign>> initialiserList;
  atl::shared_ptr<Block> constructorBlock;

  ConstructorDef(
      const atl::shared_ptr<Type> &p_classType,
      const atl::vector<atl::shared_ptr<VarDecl>> &p_constructorParams,
      const atl::vector<atl::shared_ptr<Assign>> &p_initialiserList,
      const atl::shared_ptr<Block> &p_constructorBlock);

  atl::shared_ptr<ConstructorDecl> getptr();

  bool operator==(Decl &rhs) const override;
  bool operator!=(Decl &rhs) const override;

  bool operator==(const ConstructorDef &rhs) const;
  bool operator!=(const ConstructorDef &rhs) const;

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
