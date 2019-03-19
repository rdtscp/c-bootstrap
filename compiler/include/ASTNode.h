#pragma once

#ifndef ACC_ASTNODE_H
#define ACC_ASTNODE_H

#include "atl/include/shared_ptr.h"
#include "atl/include/string.h"

#include "ASTVisitor.h"
#include "Position.h"
#include "ast/Identifier.h"
#include "targets/MIPS.h"
#include "targets/X86.h"

namespace ACC {

class Type;

class ASTNode {
public:
  virtual ~ASTNode() {}

  virtual void accept(ASTVisitor<void> &v) = 0;
  virtual atl::string accept(ASTVisitor<atl::string> &v) = 0;
  virtual atl::shared_ptr<Type>
  accept(ASTVisitor<atl::shared_ptr<Type>> &v) = 0;
  virtual MIPS::Register accept(ASTVisitor<MIPS::Register> &v) = 0;
  virtual atl::shared_ptr<X86::Operand>
  accept(ASTVisitor<atl::shared_ptr<X86::Operand>> &v) = 0;
  virtual atl::shared_ptr<ASTNode>
  accept(ASTVisitor<atl::shared_ptr<ASTNode>> &v) = 0;
  virtual atl::string astClass() const = 0;
};

}; // namespace ACC

#endif