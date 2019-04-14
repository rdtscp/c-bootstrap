#pragma once

#include "Stmt.h"
#include "Type.h"
#include "VarExpr.h"

namespace ACC {

class Deletion : public Stmt, public atl::enable_shared_from_this<Deletion> {

public:
  enum class DelType { OBJECT, ARRAY };

  DelType deletionType;
  atl::shared_ptr<VarExpr> deletionVar;

  Deletion(const DelType p_deletionType,
           const atl::shared_ptr<VarExpr> &p_deletionVar);

  bool operator==(const Deletion &rhs) const;
  bool operator!=(const Deletion &rhs) const;

  atl::shared_ptr<Deletion> getptr() { return shared_from_this(); }

  atl::string astClass() const override { return "Deletion"; }

  VISITOR_ACCEPTORS
};

} // namespace ACC
