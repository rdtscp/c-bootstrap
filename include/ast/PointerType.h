#pragma once

#ifndef POINTERTYPE_H
#define POINTERTYPE_H

#include <string>

#include "PrimitiveType.h"
#include "Type.h"

namespace ACC {

class PointerType : public Type {

public:
  std::shared_ptr<Type> pointedType;

  PointerType(std::shared_ptr<Type> pointedType) : pointedType(pointedType) {}

  bool operator==(Type &t) const override {
    if (t.astClass() == astClass())
      return *this == *static_cast<PointerType *>(&t);
    return false;
  }
  bool operator!=(Type &t) const override { return !(*this == t); }

  bool operator==(const PointerType &rhs) const {
    return *pointedType == *rhs.pointedType;
  }
  bool operator!=(const PointerType &rhs) const { return !(*this == rhs); }

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
  std::string astClass() const override { return "PointerType"; }
}; // namespace ACC
}; // namespace ACC

#endif