#pragma once

#ifndef ACC_DECL_H
#define ACC_DECL_H

#include "../ASTNode.h"

namespace ACC {

class Decl : public ASTNode {
public:
  enum class Visibility { PUBLIC, PRIVATE, PROTECTED, NONE };

  Visibility visibility;

  Decl() : visibility(Visibility::NONE) {}
  Decl(const Visibility &visibility) : visibility(visibility) {}
  virtual ~Decl() {}
  virtual bool operator==(Decl &rhs) const = 0;
  virtual bool operator!=(Decl &rhs) const = 0;
  virtual atl::shared_ptr<Identifier> getIdentifier() const = 0;
};

}; // namespace ACC

#endif