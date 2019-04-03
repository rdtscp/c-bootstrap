#pragma once

#include "Decl.h"
#include "Type.h"

namespace ACC {

class TypeDefDecl : public Decl,
                    public atl::enable_shared_from_this<TypeDefDecl> {

public:
  atl::shared_ptr<Type> type;
  atl::shared_ptr<Identifier> identifier;

  TypeDefDecl(const atl::shared_ptr<Type> &p_type,
              const atl::shared_ptr<Identifier> &p_identifier);

  atl::shared_ptr<Identifier> getIdentifier() const override;

  bool operator==(Decl &rhs) const override;
  bool operator!=(Decl &rhs) const override;

  bool operator==(const TypeDefDecl &rhs) const;
  bool operator!=(const TypeDefDecl &rhs) const;

  atl::shared_ptr<TypeDefDecl> getptr() { return shared_from_this(); }

  atl::string astClass() const override { return "TypeDefDecl"; }

  VISITOR_ACCEPTORS
};

} // namespace ACC
