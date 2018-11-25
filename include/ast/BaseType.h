#pragma once

#ifndef BASETYPE_H
#define BASETYPE_H

#include <string>

#include "PrimitiveType.h"
#include "Type.h"

namespace ACC {

class BaseType : public Type {

public:
  PrimitiveType primitiveType;

  BaseType(PrimitiveType pType) : primitiveType(pType) {}

  bool operator==(Type &t) const override {
    if (t.astClass() == astClass())
      return *this == *static_cast<BaseType *>(&t);
    return false;
  }
  bool operator!=(Type &t) const override { return !(*this == t); }

  bool operator==(const BaseType &rhs) const {
    return primitiveType == rhs.primitiveType;
  }
  bool operator!=(const BaseType &rhs) const { return !(*this == rhs); }

  void accept(ASTVisitor<void> &v) override { return v.visit(*this); }
  std::string accept(ASTVisitor<std::string> &v) override {
    return v.visit(*this);
  }
  std::shared_ptr<Type> accept(ASTVisitor<std::shared_ptr<Type>> &v) override {
    return v.visit(*this);
  }
  std::string astClass() const override { return "BaseType"; }
};

}; // namespace ACC

#endif