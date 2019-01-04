#pragma once

#ifndef BASETYPE_H
#define BASETYPE_H

#include <string>

#include "PrimitiveType.h"
#include "Type.h"

namespace ACC {

class BaseType : public Type, public std::enable_shared_from_this<BaseType> {

public:
  PrimitiveType primitiveType;

  BaseType(PrimitiveType pType);

  std::shared_ptr<BaseType> getptr();

  bool operator==(Type &rhs) const override;
  bool operator!=(Type &rhs) const override;

  bool operator==(const BaseType &rhs) const;
  bool operator!=(const BaseType &rhs) const;

  int getBytes() const override;

  void accept(ASTVisitor<void> &v) override;
  std::string accept(ASTVisitor<std::string> &v) override;
  std::shared_ptr<Type> accept(ASTVisitor<std::shared_ptr<Type>> &v) override;
  MIPS::Register accept(ASTVisitor<MIPS::Register> &v) override;
  std::shared_ptr<X86::Operand>
  accept(ASTVisitor<std::shared_ptr<X86::Operand>> &v) override;
  std::shared_ptr<ASTNode>
  accept(ASTVisitor<std::shared_ptr<ASTNode>> &v) override;
  std::string astClass() const override;
};

}; // namespace ACC

#endif