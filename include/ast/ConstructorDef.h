
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

  bool operator==(Decl &rhs) const override;
  bool operator!=(Decl &rhs) const override;

  bool operator==(const ConstructorDef &rhs) const;
  bool operator!=(const ConstructorDef &rhs) const;

  atl::shared_ptr<Identifier> getIdentifier() const override;

  atl::shared_ptr<ConstructorDecl> getptr() {
    return ConstructorDecl::shared_from_this();
  }

  atl::string astClass() const override { return "ConstructorDef"; }

  VISITOR_ACCEPTORS
};

} // namespace ACC
