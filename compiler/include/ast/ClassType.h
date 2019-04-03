#pragma once

#include "ClassTypeDecl.h"

#include "Type.h"

namespace ACC {

class ClassType : public Type, public atl::enable_shared_from_this<ClassType> {

public:
  atl::shared_ptr<Identifier> identifier;
  atl::shared_ptr<ClassTypeDecl> typeDefinition;

  ClassType(const atl::shared_ptr<Identifier> &p_identifier);

  int getBytes() const override;

  bool operator==(Type &rhs) const override;
  bool operator!=(Type &rhs) const override;

  bool operator==(const ClassType &rhs) const;
  bool operator!=(const ClassType &rhs) const;

  atl::shared_ptr<ClassType> getptr() { return shared_from_this(); }

  atl::string astClass() const override { return "ClassType"; }

  VISITOR_ACCEPTORS
};

} // namespace ACC
