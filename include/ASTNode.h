#pragma once

#ifndef ASTNODE_H
#define ASTNODE_H

#include <string>

namespace ACC {

class ASTNode {
public:
  virtual std::string astClass() const = 0;
};

}; // namespace ACC

#endif