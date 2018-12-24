#pragma once

#ifndef STRUCTTYPE_H
#define STRUCTTYPE_H

#include <memory>
#include <string>

#include "Type.h"
#include "StructTypeDecl.h"

namespace ACC {

class StructType : public Type {

public:
  std::string identifier;
  std::shared_ptr<StructTypeDecl> typeDefinition;

  StructType(const std::string &identifier) : identifier(identifier) {}

  int getBytes() const override; /* Defined in StructTypeDecl.h */

  bool operator==(Type &rhs) const override {
    if (rhs.astClass() == astClass())
      return *this == *static_cast<StructType *>(&rhs);
    return false;
  }
  bool operator!=(Type &rhs) const override { return !(*this == rhs); }

  bool operator==(const StructType &rhs) const {
    return identifier == rhs.identifier;
  }
  bool operator!=(const StructType &rhs) const { return !(*this == rhs); }

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
  X86::Register accept(ASTVisitor<X86::Register> &v) override {
    return v.visit(*this);
  }
  std::string astClass() const override { return "StructType"; }
};

}; // namespace ACC

#endif