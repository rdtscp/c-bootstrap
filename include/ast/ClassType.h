#pragma once

#include "ClassTypeDef.h"
#include "Type.h"

namespace ACC {

class ClassTypeDef;

class ClassType : public Type, public atl::enable_shared_from_this<ClassType> {

public:
  atl::shared_ptr<Identifier> identifier;
  atl::shared_ptr<ClassTypeDef> typeDefinition;

  ClassType(const atl::shared_ptr<Identifier> &p_identifier);

  virtual bool canCastTo(Type &rhs) const override;
  virtual bool equivalentTo(Type &rhs) const override; //{ return *this == rhs; }
  unsigned int getBytes() const override;

  bool operator==(Type &rhs) const override;
  bool operator!=(Type &rhs) const override;

  bool operator==(const ClassType &rhs) const;
  bool operator!=(const ClassType &rhs) const;

  atl::shared_ptr<ClassType> getptr() { return shared_from_this(); }

  atl::string astClass() const override { return "ClassType"; }

  VISITOR_ACCEPTORS
};

} // namespace ACC
