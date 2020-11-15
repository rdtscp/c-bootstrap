#pragma once

#include "Block.h"
#include "Scope.h"

namespace ACC {

class Namespace : public Decl,
                  public Scope,
                  public atl::enable_shared_from_this<Namespace> {

public:
  unsigned int namespaceDeclsChecked = 0;
  atl::shared_ptr<Identifier> identifier;
  atl::vector<atl::shared_ptr<Decl>> namespaceDecls;

  Namespace(const atl::shared_ptr<Identifier> &p_identifier,
            const atl::vector<atl::shared_ptr<Decl>> &p_namespaceDecls);

  atl::shared_ptr<Identifier> getIdentifier() const override;

  bool operator==(Decl &rhs) const override;
  bool operator!=(Decl &rhs) const override;

  bool operator==(const Namespace &rhs) const;
  bool operator!=(const Namespace &rhs) const;

  atl::shared_ptr<Namespace> getptr() { return shared_from_this(); }

  atl::string astClass() const override { return "Namespace"; }

  SCOPE_OVERRIDES

  VISITOR_ACCEPTORS
};
} // namespace ACC
