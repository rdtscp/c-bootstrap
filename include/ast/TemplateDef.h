#pragma once

#include "Decl.h"

namespace ACC {

class TemplateDef : public Decl,
                    public atl::enable_shared_from_this<TemplateDef> {
public:
  atl::vector<atl::shared_ptr<Identifier>> templateParams;
  atl::shared_ptr<Decl> templatedDecl;

  TemplateDef(const atl::vector<atl::shared_ptr<Identifier>> &params,
              const atl::shared_ptr<Decl> &decl);

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
