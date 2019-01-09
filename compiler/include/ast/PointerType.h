#pragma once

#ifndef ACC_POINTERTYPE_H
#define ACC_POINTERTYPE_H

#include <string>

#include "PrimitiveType.h"
#include "Type.h"

namespace ACC {

class PointerType : public Type,
                    public std::enable_shared_from_this<PointerType> {

public:
  std::shared_ptr<Type> pointedType;

  PointerType(std::shared_ptr<Type> pointedType);

  std::shared_ptr<PointerType> getptr();

  bool operator==(Type &rhs) const override;

  bool operator!=(Type &rhs) const override;

  bool operator==(const PointerType &rhs) const;
  bool operator!=(const PointerType &rhs) const;

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