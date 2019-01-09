#pragma once

#ifndef ACC_DECL_H
#define ACC_DECL_H

#include "../ASTNode.h"

namespace ACC {

class Decl : public ASTNode {
public:
  virtual bool operator==(Decl &rhs) const = 0;
  virtual bool operator!=(Decl &rhs) const = 0;
  virtual std::string getIdentifier() const = 0;
};

}; // namespace ACC

#endif