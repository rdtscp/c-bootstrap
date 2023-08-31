#pragma once

#include "atl/include/shared_ptr.h"
#include "atl/include/string.h"

#include "ASTNode.h"

#include "Type.h"

namespace ACC {

class Identifier : public ASTNode,
                   public atl::enable_shared_from_this<Identifier> {

public:
  atl::string value;
  atl::shared_ptr<Identifier> tailIdentifier;

  Identifier(const atl::string &p_value);

  atl::shared_ptr<Identifier> clone() const;

  void insert(const atl::shared_ptr<Identifier> &p_tailIdentifier);

  const atl::string head() const;
  const atl::shared_ptr<Identifier> tail() const;
  const unsigned int size() const;

  atl::string mangle() const;
  atl::string toString() const;

  atl::shared_ptr<Identifier> getptr() { return shared_from_this(); }

  bool operator==(const Identifier &rhs) const;
  bool operator!=(const Identifier &rhs) const;

  atl::string astClass() const override { return "Identifier"; }

  VISITOR_ACCEPTORS
};

} // namespace ACC
