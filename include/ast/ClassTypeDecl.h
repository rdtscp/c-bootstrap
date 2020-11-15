#pragma once

#include "Decl.h"
#include "Scope.h"

namespace ACC {

class ClassTypeDecl : public Decl,
                      public Scope,
                      public atl::enable_shared_from_this<ClassTypeDecl> {

public:
  atl::shared_ptr<Identifier> classIdentifier;

  ClassTypeDecl(const atl::shared_ptr<Identifier> &p_classIdentifier);

  atl::shared_ptr<Identifier> getIdentifier() const override;

  bool operator==(Decl &rhs) const override;
  bool operator!=(Decl &rhs) const override;

  bool operator==(const ClassTypeDecl &rhs) const;
  bool operator!=(const ClassTypeDecl &rhs) const;

  atl::shared_ptr<ClassTypeDecl> getptr() { return shared_from_this(); }

  atl::string astClass() const override { return "ClassTypeDecl"; }

  SCOPE_OVERRIDES

  VISITOR_ACCEPTORS
};

} // namespace ACC
