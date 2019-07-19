#pragma once

#include "Decl.h"
#include "Expr.h"
#include "Stmt.h"
#include "Type.h"
#include "VarDecl.h"

namespace ACC {

class VarDef : public VarDecl {

public:
  atl::shared_ptr<Expr> varValue;

  VarDef(const atl::shared_ptr<Type> &p_varType,
         const atl::shared_ptr<Identifier> &p_varidentifier,
         const atl::shared_ptr<Expr> &p_varValue);

  unsigned int getBytes() const;

  atl::shared_ptr<Identifier> getIdentifier() const override;

  bool operator==(Decl &rhs) const override;
  bool operator!=(Decl &rhs) const override;

  bool operator==(const VarDef &rhs) const;
  bool operator!=(const VarDef &rhs) const;

  atl::shared_ptr<VarDecl> getptr() { return VarDecl::shared_from_this(); }

  atl::string astClass() const override { return "VarDef"; }

  VISITOR_ACCEPTORS
};

} // namespace ACC
