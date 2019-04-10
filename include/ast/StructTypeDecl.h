#pragma once

#include "Decl.h"
#include "Scope.h"
#include "StructType.h"
#include "VarDecl.h"

namespace ACC {

class StructTypeDecl : public Decl,
                       public Scope,
                       public atl::enable_shared_from_this<StructTypeDecl> {

public:
  atl::shared_ptr<StructType> structType;
  atl::vector<atl::shared_ptr<VarDecl>> varDecls;
  int allocSize;
  int compactSize;

  StructTypeDecl(const atl::shared_ptr<StructType> &p_structType,
                 const atl::vector<atl::shared_ptr<VarDecl>> &p_varDecls);

  atl::shared_ptr<Identifier> getIdentifier() const override;

  bool operator==(Decl &rhs) const override;
  bool operator!=(Decl &rhs) const override;

  bool operator==(const StructTypeDecl &rhs) const;
  bool operator!=(const StructTypeDecl &rhs) const;

  atl::shared_ptr<StructTypeDecl> getptr() { return shared_from_this(); }

  atl::string astClass() const override { return "StructTypeDecl"; }

  VISITOR_ACCEPTORS
};

} // namespace ACC
