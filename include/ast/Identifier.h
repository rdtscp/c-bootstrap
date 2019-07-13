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
  atl::shared_ptr<Identifier> parentIdentifier;

  Identifier();

  Identifier(const atl::string &p_value);

  Identifier(const atl::string &p_value,
             const atl::shared_ptr<Identifier> &p_parentIdentifier);

  Identifier(const Identifier &) = delete;

  const unsigned int namespaceCount() const;
  const atl::shared_ptr<Identifier> namespaceHead() const;
  const atl::shared_ptr<Identifier> namespaceTail() const;

  atl::string mangle() const;
  atl::string toString() const;

  atl::shared_ptr<Identifier> getptr() { return shared_from_this(); }

  bool operator==(const Identifier &rhs) const;
  bool operator!=(const Identifier &rhs) const;

  atl::string astClass() const override { return "Identifier"; }

  VISITOR_ACCEPTORS

private:
  const atl::shared_ptr<Identifier>
  deepCopyIdentifier(const unsigned int depth) const;
};

} // namespace ACC
