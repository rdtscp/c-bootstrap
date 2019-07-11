#pragma once

#include "Decl.h"

namespace ACC {

class TemplateDef : public Decl,
                    public atl::enable_shared_from_this<TemplateDef> {
public:
  TemplateDef();

  atl::shared_ptr<Identifier> getIdentifier() const override;

  bool operator==(Decl &rhs) const override;
  bool operator!=(Decl &rhs) const override;

  bool operator==(const TemplateDef &rhs) const;
  bool operator!=(const TemplateDef &rhs) const;

  atl::shared_ptr<TemplateDef> getptr() { return shared_from_this(); }

  atl::string astClass() const override { return "TemplateDef"; }

  VISITOR_ACCEPTORS
};

} // namespace ACC
