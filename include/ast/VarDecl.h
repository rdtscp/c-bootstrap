#pragma once

#include "Decl.h"
#include "FunDecl.h"
#include "Stmt.h"
#include "Type.h"

namespace ACC {

class FunDecl;

class VarDecl : public Decl,
                public Stmt,
                public atl::enable_shared_from_this<VarDecl> {

public:
  atl::shared_ptr<Type> type;
  atl::shared_ptr<Identifier> identifier;
  atl::shared_ptr<FunDecl> parentFunc;

  int fpOffset = 0;

  VarDecl(const atl::shared_ptr<Type> &p_type,
          const atl::shared_ptr<Identifier> &p_identifier);

  unsigned int getBytes() const;

  atl::shared_ptr<Identifier> getIdentifier() const override;

  bool operator==(Decl &rhs) const override;
  bool operator!=(Decl &rhs) const override;

  bool operator==(const VarDecl &rhs) const;
  bool operator!=(const VarDecl &rhs) const;

  atl::shared_ptr<VarDecl> getptr() { return shared_from_this(); }

  atl::string astClass() const override { return "VarDecl"; }

  VISITOR_ACCEPTORS
};

} // namespace ACC
