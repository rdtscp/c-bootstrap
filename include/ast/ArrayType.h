#pragma once

#ifndef ARRAYTYPE_H
#define ARRAYTYPE_H

#include <memory>
#include <string>

#include "Type.h"

namespace ACC {

class ArrayType : public Type {

public:
  std::string arraySize;
  std::shared_ptr<Type> arrayType;

  ArrayType(std::shared_ptr<Type> arrayType, const std::string &arraySize)
      : arraySize(arraySize), arrayType(arrayType) {}

  bool operator==(Type &t) const override {
    if (t.astClass() == astClass())
      return *this == *static_cast<ArrayType *>(&t);
    return false;
  }
  bool operator!=(Type &t) const override { return !(*this == t); }

  bool operator==(const ArrayType &rhs) const {
    return (*arrayType == *rhs.arrayType && arraySize == rhs.arraySize);
  }
  bool operator!=(const ArrayType &rhs) const { return !(*this == rhs); }

  void accept(ASTVisitor<void> &v) override { return v.visit(*this); }
  std::string accept(ASTVisitor<std::string> &v) override {
    return v.visit(*this);
  }
  std::shared_ptr<Type> accept(ASTVisitor<std::shared_ptr<Type>> &v) override {
    return v.visit(*this);
  }
  MIPS::Register accept(ASTVisitor<MIPS::Register> &v) override {
    return v.visit(*this);
  }
  std::string astClass() const override { return "ArrayType"; }
};

}; // namespace ACC

#endif