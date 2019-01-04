#pragma once

#ifndef ENUMTYPE_H
#define ENUMTYPE_H

#include <map>
#include <string>

#include "PrimitiveType.h"
#include "Type.h"

namespace ACC {

class EnumType : public Type, public std::enable_shared_from_this<EnumType> {

public:
  std::map<std::string, unsigned int> states;

  EnumType(std::map<std::string, unsigned int> states);

  std::shared_ptr<EnumType> getptr();

  bool operator==(Type &t) const override;

  bool operator!=(Type &t) const override;

  bool operator==(const EnumType &rhs) const;
  bool operator!=(const EnumType &rhs) const;

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