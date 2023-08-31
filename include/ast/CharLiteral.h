#pragma once

#include "Expr.h"
#include "Literal.h"
#include "Type.h"

namespace ACC {

class CharLiteral : public Literal,
                    public atl::enable_shared_from_this<CharLiteral> {
public:
  CharLiteral(const atl::string &p_literal);

  
  bool operator==(Expr &rhs) const override;
  bool operator!=(Expr &rhs) const override;

  bool operator==(Literal &rhs) const override;
  bool operator!=(Literal &rhs) const override;

  bool operator==(const CharLiteral &rhs) const;
  bool operator!=(const CharLiteral &rhs) const;

  bool operator==(const Stmt &rhs) const override;
  bool operator!=(const Stmt &rhs) const override;

  atl::shared_ptr<CharLiteral> getptr() { return shared_from_this(); }

  atl::string astClass() const override { return "CharLiteral"; }

  VISITOR_ACCEPTORS
};

} // namespace ACC
