#pragma once

#include "ClassType.h"
#include "Scope.h"
#include "VarDecl.h"

namespace ACC {

class ConstructorDecl : public Decl,
                        public Scope,
                        public atl::enable_shared_from_this<ConstructorDecl> {

public:
  atl::shared_ptr<ClassType> classType;
  atl::vector<atl::shared_ptr<VarDecl>> constructorParams;

  ConstructorDecl(
      const atl::shared_ptr<Type> &p_classType,
      const atl::vector<atl::shared_ptr<VarDecl>> &p_constructorParams);

  virtual ~ConstructorDecl() {}

  atl::shared_ptr<Identifier> getIdentifier() const override;

  bool operator==(Decl &rhs) const override;
  bool operator!=(Decl &rhs) const override;

  bool operator==(const ConstructorDecl &rhs) const;
  bool operator!=(const ConstructorDecl &rhs) const;

  atl::shared_ptr<ConstructorDecl> getptr() { return shared_from_this(); }

  atl::string astClass() const override { return "ConstructorDecl"; }

  VISITOR_ACCEPTORS
};

} // namespace ACC
