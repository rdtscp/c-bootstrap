#pragma once

#ifndef STRUCTTYPE_H
#define STRUCTTYPE_H

#include <memory>
#include <string>

#include "StructTypeDecl.h"
#include "Type.h"

namespace ACC {

class StructType : public Type {

public:
  std::string identifier;
  std::shared_ptr<StructTypeDecl> typeDefinition;

  StructType(const std::string &identifier);

  int getBytes() const override;

  bool operator==(Type &rhs) const override;
  bool operator!=(Type &rhs) const override;

  bool operator==(const StructType &rhs) const;
  bool operator!=(const StructType &rhs) const;

  void accept(ASTVisitor<void> &v) override;
  std::string accept(ASTVisitor<std::string> &v) override;
  std::shared_ptr<Type> accept(ASTVisitor<std::shared_ptr<Type>> &v) override;
  MIPS::Register accept(ASTVisitor<MIPS::Register> &v) override;
  std::shared_ptr<X86::Operand> accept(ASTVisitor<std::shared_ptr<X86::Operand>> &v) override;
  std::string astClass() const override;
};

}; // namespace ACC

#endif