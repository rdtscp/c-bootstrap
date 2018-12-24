#pragma once

#ifndef ASTNODE_H
#define ASTNODE_H

#include <string>

#include "ASTVisitor.h"
#include "Position.h"
#include "targets/MIPS.h"
#include "targets/X86.h"

namespace ACC {

class Type;

class ASTNode {
public:
  Position position;

  virtual void accept(ASTVisitor<void> &v) = 0;
  virtual std::string accept(ASTVisitor<std::string> &v) = 0;
  virtual std::shared_ptr<Type>
  accept(ASTVisitor<std::shared_ptr<Type>> &v) = 0;
  virtual MIPS::Register accept(ASTVisitor<MIPS::Register> &v) = 0;
  virtual X86::Register accept(ASTVisitor<X86::Register> &v) = 0;
  virtual std::string astClass() const = 0;
};

}; // namespace ACC

#endif