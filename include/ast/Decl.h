#pragma once

#include "ASTNode.h"
#include "Identifier.h"

namespace ACC {

class Decl : public ASTNode {
public:
  enum class Visibility { PUBLIC, PRIVATE, PROTECTED, NONE };

  Visibility visibility;

  Decl() : visibility(Visibility::NONE) {}
  Decl(const Visibility &p_visibility) : visibility(p_visibility) {}
  virtual ~Decl() {}
  virtual bool operator==(Decl &rhs) const = 0;
  virtual bool operator!=(Decl &rhs) const = 0;
  virtual atl::shared_ptr<Identifier> getIdentifier() const = 0;
};

} // namespace ACC
