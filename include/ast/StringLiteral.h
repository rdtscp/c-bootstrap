#pragma once

#include "Expr.h"
#include "Literal.h"
#include "Type.h"

namespace ACC {

class StringLiteral : public Literal,
                      public atl::enable_shared_from_this<StringLiteral> {
public:
  StringLiteral(const atl::string &p_literal);

  atl::string getSignature() const override;

  bool operator==(Expr &rhs) const override;
  bool operator!=(Expr &rhs) const override;

  bool operator==(Literal &rhs) const override;
  bool operator!=(Literal &rhs) const override;

  bool operator==(const StringLiteral &rhs) const;
  bool operator!=(const StringLiteral &rhs) const;

  atl::shared_ptr<StringLiteral> getptr() { return shared_from_this(); }

  atl::string astClass() const override { return "StringLiteral"; }

  VISITOR_ACCEPTORS
};

} // namespace ACC
