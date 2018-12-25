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

  BaseType(PrimitiveType pType);
  
  bool operator==(Type &t) const override;
  bool operator!=(Type &t) const override;

  bool operator==(const BaseType &rhs) const;
  bool operator!=(const BaseType &rhs) const;

  int getBytes() const override;

  void accept(ASTVisitor<void> &v) override;
  std::string accept(ASTVisitor<std::string> &v) override;
  std::shared_ptr<Type> accept(ASTVisitor<std::shared_ptr<Type>> &v) override;
  MIPS::Register accept(ASTVisitor<MIPS::Register> &v) override;
  X86::Register accept(ASTVisitor<X86::Register> &v) override;
  std::string astClass() const override;
};

}; // namespace ACC

#endif