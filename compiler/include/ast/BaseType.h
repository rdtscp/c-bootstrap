#pragma once

#ifndef ACC_BASETYPE_H
#define ACC_BASETYPE_H

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
  atl::string accept(ASTVisitor<atl::string> &v) override;
  std::shared_ptr<Type> accept(ASTVisitor<std::shared_ptr<Type>> &v) override;
  MIPS::Register accept(ASTVisitor<MIPS::Register> &v) override;
  std::shared_ptr<X86::Operand>
  accept(ASTVisitor<std::shared_ptr<X86::Operand>> &v) override;
  std::shared_ptr<ASTNode>
  accept(ASTVisitor<std::shared_ptr<ASTNode>> &v) override;
  atl::string astClass() const override;
};

}; // namespace ACC

#endif