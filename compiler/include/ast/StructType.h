#pragma once

#include "StructTypeDecl.h"
#include "Type.h"

namespace ACC {

class StructType : public Type,
                   public atl::enable_shared_from_this<StructType> {

public:
  atl::shared_ptr<Identifier> identifier;
  atl::shared_ptr<StructTypeDecl> typeDefinition;

  StructType(const atl::shared_ptr<Identifier> &p_identifier);

  int getBytes() const override;

  bool operator==(Type &rhs) const override;
  bool operator!=(Type &rhs) const override;

  bool operator==(const StructType &rhs) const;
  bool operator!=(const StructType &rhs) const;

  atl::shared_ptr<StructType> getptr() { return shared_from_this(); }

  atl::string astClass() const override { return "StructType"; }

  VISITOR_ACCEPTORS
};

} // namespace ACC
