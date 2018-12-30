#pragma once

#ifndef ARRAYTYPE_H
#define ARRAYTYPE_H

#include <memory>
#include <string>

#include "Type.h"

namespace ACC {

class ArrayType : public Type, public std::enable_shared_from_this<ArrayType> {

public:
  std::string arraySize;
  std::shared_ptr<Type> arrayType;

  ArrayType(std::shared_ptr<Type> arrayType, const std::string &arraySize);

  std::shared_ptr<ArrayType> getptr();

  bool operator==(Type &t) const override;
  bool operator!=(Type &t) const override;

  bool operator==(const ArrayType &rhs) const;
  bool operator!=(const ArrayType &rhs) const;

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