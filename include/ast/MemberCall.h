
#pragma once

#include "Expr.h"
#include "FunCall.h"
#include "SourceToken.h"

namespace ACC {

class MemberCall : public Expr,
                   public atl::enable_shared_from_this<MemberCall> {

public:
  SourceToken::Class accessType;
  atl::shared_ptr<Expr> object;
  atl::shared_ptr<FunCall> funCall;

  MemberCall(const atl::shared_ptr<Expr> &p_object,
             const atl::shared_ptr<FunCall> &p_funCall,
             const SourceToken::Class &p_accessType);

  
  bool operator==(Expr &rhs) const override;
  bool operator!=(Expr &rhs) const override;

  bool operator==(const MemberCall &rhs) const;
  bool operator!=(const MemberCall &rhs) const;

  bool operator==(const Stmt &rhs) const override;
  bool operator!=(const Stmt &rhs) const override;

  atl::shared_ptr<MemberCall> getptr() { return shared_from_this(); }

  atl::string astClass() const override { return "MemberCall"; }

  VISITOR_ACCEPTORS
};

} // namespace ACC
