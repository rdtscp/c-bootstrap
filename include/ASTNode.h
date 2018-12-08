#pragma once

#ifndef ASTNODE_H
#define ASTNODE_H

#include <string>

#include "ASTVisitor.h"
#include "targets/Registers.h"

namespace ACC {

class Type;

class ASTNode {
public:
  std::string file;
  std::string line;
  std::string column;

  virtual void accept(ASTVisitor<void> &v) = 0;
  virtual std::string accept(ASTVisitor<std::string> &v) = 0;
  virtual std::shared_ptr<Type>
  accept(ASTVisitor<std::shared_ptr<Type>> &v) = 0;
  virtual MIPS::Register accept(ASTVisitor<MIPS::Register> &v) = 0;
  virtual std::string astClass() const = 0;
};

}; // namespace ACC

#endif