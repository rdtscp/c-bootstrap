#pragma once

#ifndef STRUCTTYPE_H
#define STRUCTTYPE_H

#include <memory>
#include <string>

#include "Type.h"

namespace ACC {

class StructType : public Type {

public:
  std::string identifier;

  StructType(const std::string &identifier) : identifier(identifier) {}

  bool operator==(Type &rhs) const override {
    if (rhs.astClass() == astClass())
      return *this == *static_cast<StructType *>(&rhs);
    return false;
  }
  bool operator!=(Type &rhs) const override { return !(*this == rhs); }

  bool operator==(const StructType &rhs) const { return identifier == rhs.identifier; }
  bool operator!=(const StructType &rhs) const { return !(*this==rhs); }

  std::string accept(ASTVisitor &v) override { return v.visit(*this); }
  std::string astClass() const override { return "StructType"; }
};

}; // namespace ACC

#endif