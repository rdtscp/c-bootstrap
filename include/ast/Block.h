#pragma once

#include "Decl.h"
#include "Scope.h"
#include "Stmt.h"
#include "Type.h"

namespace ACC {

class Block : public Stmt,
              public Scope,
              public atl::enable_shared_from_this<Block> {

public:
  atl::vector<atl::shared_ptr<Stmt>> stmts;

  Block(const atl::vector<atl::shared_ptr<Stmt>> &p_stmts);

  bool operator==(const Block &rhs) const;
  bool operator!=(const Block &rhs) const;

  atl::shared_ptr<Block> getptr() { return shared_from_this(); }

  atl::string astClass() const override { return "Block"; }

  VISITOR_ACCEPTORS
};

} // namespace ACC
