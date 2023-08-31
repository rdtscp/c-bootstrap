#pragma once

#include "Stmt.h"
#include "StringLiteral.h"
#include "Type.h"

namespace ACC {

class Throw : public Stmt, public atl::enable_shared_from_this<Throw> {

public:
  atl::shared_ptr<StringLiteral> exceptionText;

  Throw();
  Throw(const atl::shared_ptr<StringLiteral> &p_exceptionText);

  bool operator==(const Stmt &rhs) const override;
  bool operator!=(const Stmt &rhs) const override;

  bool operator==(const Throw &rhs) const;
  bool operator!=(const Throw &rhs) const;

  atl::shared_ptr<Throw> getptr() { return shared_from_this(); }

  atl::string astClass() const override { return "Throw"; }

  VISITOR_ACCEPTORS
};

} // namespace ACC
