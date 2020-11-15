#pragma once

#include "FunCall.h"

namespace ACC {

class TemplatedFunCall : public FunCall {

public:
  atl::vector<atl::shared_ptr<Type>> templateArgs;

  TemplatedFunCall(const atl::shared_ptr<Identifier> &p_funIdentifier,
                   const atl::vector<atl::shared_ptr<Expr>> &p_funArgs,
                   const atl::vector<atl::shared_ptr<Type>> &p_templateArgs);

  bool operator==(Expr &rhs) const override;
  bool operator!=(Expr &rhs) const override;

  bool operator==(const TemplatedFunCall &rhs) const;
  bool operator!=(const TemplatedFunCall &rhs) const;

  bool operator==(const Stmt &rhs) const override;
  bool operator!=(const Stmt &rhs) const override;

  atl::string astClass() const override { return "TemplatedFunCall"; }

  VISITOR_ACCEPTORS
};

} // namespace ACC
