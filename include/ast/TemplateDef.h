#pragma once

#include "Decl.h"
#include "FunSignature.h"

namespace ACC {

class TemplateDef : public Decl,
                    public atl::enable_shared_from_this<TemplateDef> {
public:
  atl::vector<atl::shared_ptr<Identifier>> templateParams;
  atl::shared_ptr<Decl> templatedDecl;

  atl::vector<atl::shared_ptr<Decl>> instantiatedDecl;

  TemplateDef(const atl::vector<atl::shared_ptr<Identifier>> &params,
              const atl::shared_ptr<Decl> &decl);

  atl::shared_ptr<Identifier> getIdentifier() const override;

  bool operator==(Decl &rhs) const override;
  bool operator!=(Decl &rhs) const override;

  bool operator==(const TemplateDef &rhs) const;
  bool operator!=(const TemplateDef &rhs) const;

  atl::shared_ptr<TemplateDef> getptr() { return shared_from_this(); }

  atl::string astClass() const override { return "TemplateDef"; }

  atl::shared_ptr<FunDecl> findFunDecl(const FunSignature &funSignature,
                                       const atl::shared_ptr<Decl> &exemptDecl);

  VISITOR_ACCEPTORS
};

} // namespace ACC
