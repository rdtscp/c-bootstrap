#pragma once

#include "ClassTypeDecl.h"

namespace ACC {

class ClassTypeDef : public ClassTypeDecl {

public:
  atl::vector<atl::shared_ptr<Decl>> classDecls;

  ClassTypeDef(const atl::shared_ptr<ClassType> &p_classType,
               const atl::vector<atl::shared_ptr<Decl>> &p_classDecls);

  atl::shared_ptr<Identifier> getIdentifier() const override;

  bool operator==(Decl &rhs) const override;
  bool operator!=(Decl &rhs) const override;

  bool operator==(const ClassTypeDef &rhs) const;
  bool operator!=(const ClassTypeDef &rhs) const;

  atl::shared_ptr<ClassTypeDecl> getptr() {
    return ClassTypeDecl::shared_from_this();
  }

  atl::string astClass() const override { return "ClassTypeDef"; }

  VISITOR_ACCEPTORS
};

} // namespace ACC
