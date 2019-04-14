#pragma once

#include "ClassType.h"
#include "Decl.h"
#include "Scope.h"
#include "VarDecl.h"

namespace ACC {

class ClassTypeDecl : public Decl,
                      public Scope,
                      public atl::enable_shared_from_this<ClassTypeDecl> {

public:
  atl::shared_ptr<ClassType> classType;
  atl::vector<atl::shared_ptr<Decl>> classDecls;

  ClassTypeDecl(const atl::shared_ptr<ClassType> &p_classType,
                const atl::vector<atl::shared_ptr<Decl>> &p_classDecls);

  atl::shared_ptr<Identifier> getIdentifier() const override;

  bool operator==(Decl &rhs) const override;
  bool operator!=(Decl &rhs) const override;

  bool operator==(const ClassTypeDecl &rhs) const;
  bool operator!=(const ClassTypeDecl &rhs) const;

  atl::shared_ptr<ClassTypeDecl> getptr() { return shared_from_this(); }

  atl::string astClass() const override { return "ClassTypeDecl"; }

  VISITOR_ACCEPTORS
};

} // namespace ACC
