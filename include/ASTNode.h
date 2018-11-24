#pragma once

#ifndef ASTNODE_H
#define ASTNODE_H

#include <string>

#include "ASTVisitor.h"

namespace ACC {

class ASTNode {
public:
  virtual std::string accept(ASTVisitor &v) = 0;
  virtual std::string astClass() const = 0;
};

}; // namespace ACC

#endif