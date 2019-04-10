#pragma once

#include "ClassType.h"
// #include "Decl.h "
#include "VarDecl.h"

namespace ACC {

class DestructorDecl : public Decl,
                       public atl::enable_shared_from_this<DestructorDecl> {

public:
  atl::shared_ptr<ClassType> classType;

  DestructorDecl(const atl::shared_ptr<Type> &p_classType);

  atl::shared_ptr<Identifier> getIdentifier() const override;

  bool operator==(Decl &rhs) const override;
  bool operator!=(Decl &rhs) const override;

  bool operator==(const DestructorDecl &rhs) const;
  bool operator!=(const DestructorDecl &rhs) const;

  atl::shared_ptr<DestructorDecl> getptr() { return shared_from_this(); }

  atl::string astClass() const override { return "DestructorDecl"; }

  VISITOR_ACCEPTORS
};

} // namespace ACC
