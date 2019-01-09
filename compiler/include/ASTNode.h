#pragma once

#ifndef ACC_ASTNODE_H
#define ACC_ASTNODE_H

#include <string>

#include "ASTVisitor.h"
#include "Position.h"
#include "targets/MIPS.h"
#include "targets/X86.h"

namespace ACC {

class Type;

class ASTNode {
public:
  // Position position;

  virtual void accept(ASTVisitor<void> &v) = 0;
  virtual std::string accept(ASTVisitor<std::string> &v) = 0;
  virtual std::shared_ptr<Type>
  accept(ASTVisitor<std::shared_ptr<Type>> &v) = 0;
  virtual MIPS::Register accept(ASTVisitor<MIPS::Register> &v) = 0;
  virtual std::shared_ptr<X86::Operand>
  accept(ASTVisitor<std::shared_ptr<X86::Operand>> &v) = 0;
  virtual std::shared_ptr<ASTNode>
  accept(ASTVisitor<std::shared_ptr<ASTNode>> &v) = 0;
  virtual std::string astClass() const = 0;
};

}; // namespace ACC

#endif