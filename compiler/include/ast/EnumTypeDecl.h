#pragma once

#include <map>

#include "Decl.h"
#include "PrimitiveType.h"
#include "Type.h"

namespace ACC {

class EnumTypeDecl : public Decl,
                     public Type,
                     public atl::enable_shared_from_this<EnumTypeDecl> {

public:
  atl::shared_ptr<Identifier> identifier;
  std::map<std::string, std::string> states;

  EnumTypeDecl(const atl::shared_ptr<Identifier> &p_identifier,
               const std::map<std::string, std::string> &p_states);

  int getBytes() const override;

  atl::shared_ptr<Identifier> getIdentifier() const override;

  bool operator==(Decl &rhs) const override;
  bool operator!=(Decl &rhs) const override;

  bool operator==(Type &rhs) const override;
  bool operator!=(Type &rhs) const override;

  bool operator==(const EnumTypeDecl &rhs) const;
  bool operator!=(const EnumTypeDecl &rhs) const;

  atl::shared_ptr<EnumTypeDecl> getptr() { return shared_from_this(); }

  atl::string astClass() const override { return "EnumTypeDecl"; }

  VISITOR_ACCEPTORS
};

} // namespace ACC
