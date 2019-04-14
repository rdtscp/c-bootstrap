#pragma once

#include "Expr.h"
#include "Identifier.h"
#include "Type.h"

namespace ACC {

class MemberAccess : public Expr,
                     public atl::enable_shared_from_this<MemberAccess> {

public:
  atl::shared_ptr<Expr> object;
  atl::shared_ptr<Identifier> fieldIdentifier;

  MemberAccess(const atl::shared_ptr<Expr> &p_object,
               const atl::shared_ptr<Identifier> &p_fieldIdentifier);

  bool operator==(Expr &rhs) const override;
  bool operator!=(Expr &rhs) const override;

  bool operator==(const MemberAccess &rhs) const;
  bool operator!=(const MemberAccess &rhs) const;

  atl::shared_ptr<MemberAccess> getptr() { return shared_from_this(); }

  atl::string astClass() const override { return "MemberAccess"; }

  VISITOR_ACCEPTORS
};

} // namespace ACC
