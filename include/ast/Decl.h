#pragma once

#ifndef DECL_H
#define DECL_H

#include "../ASTNode.h"

namespace ACC {

class Decl : public ASTNode {
public:
  virtual bool operator==(Decl &rhs) const = 0;
  virtual bool operator!=(Decl &rhs) const = 0;
};

}; // namespace ACC

#endif