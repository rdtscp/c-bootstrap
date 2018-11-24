#pragma once

#ifndef TYPE_H
#define TYPE_H

#include "../ASTNode.h"

namespace ACC {

class Type : public ASTNode {
public:
  virtual bool operator==(Type &rhs) const = 0;
  virtual bool operator!=(Type &rhs) const = 0;
};

}; // namespace ACC

#endif