#pragma once

#include <map>
#include <string>

#include "Decl.h"
#include "PrimitiveType.h"
#include "Type.h"

namespace ACC {

class EnumClassTypeDecl
    : public Decl,
      public atl::enable_shared_from_this<EnumClassTypeDecl> {

public:
  atl::shared_ptr<Identifier> identifier;
  std::map<std::string, std::string> states;

  EnumClassTypeDecl(const atl::shared_ptr<Identifier> &p_identifier,
                    const std::map<std::string, std::string> &p_states);

  atl::shared_ptr<Identifier> getIdentifier() const override;

  bool operator==(Decl &rhs) const override;
  bool operator!=(Decl &rhs) const override;

  bool operator==(const EnumClassTypeDecl &rhs) const;
  bool operator!=(const EnumClassTypeDecl &rhs) const;

  atl::shared_ptr<EnumClassTypeDecl> getptr() { return shared_from_this(); }

  atl::string astClass() const override { return "EnumClassTypeDecl"; }

  VISITOR_ACCEPTORS
};

} // namespace ACC
