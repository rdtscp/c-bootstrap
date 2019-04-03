#pragma once

#include "Block.h"
#include "Decl.h"
#include "FunDecl.h"
#include "Type.h"
#include "VarDecl.h"

namespace ACC {

class FunDef : public FunDecl {

public:
  atl::shared_ptr<Block> funBlock;

  FunDef(const atl::set<FunModifiers> &p_funModifiers,
         const atl::shared_ptr<Identifier> &p_funIdentifier,
         const atl::vector<atl::shared_ptr<VarDecl>> &p_funParams,
         const atl::shared_ptr<Type> &p_funType,
         const atl::shared_ptr<Block> &p_funBlock);

  atl::shared_ptr<Identifier> getIdentifier() const override;

  bool operator==(Decl &rhs) const override;
  bool operator!=(Decl &rhs) const override;

  bool operator==(const FunDecl &rhs) const;
  bool operator!=(const FunDecl &rhs) const;

  bool operator==(const FunDef &rhs) const;
  bool operator!=(const FunDef &rhs) const;

  atl::shared_ptr<FunDecl> getptr() { return FunDecl::shared_from_this(); }

  atl::string astClass() const override { return "FunDef"; }

  VISITOR_ACCEPTORS
};

} // namespace ACC
