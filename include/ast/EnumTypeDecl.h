#pragma once

#ifndef ENUMTYPEDECL_H
#define ENUMTYPEDECL_H

#include <map>
#include <string>

#include "Decl.h"
#include "PrimitiveType.h"

namespace ACC {

class EnumTypeDecl : public Decl,
                     public std::enable_shared_from_this<EnumTypeDecl> {

public:
  std::map<std::string, unsigned int> states;

  EnumTypeDecl(std::map<std::string, unsigned int> states);

  std::shared_ptr<EnumTypeDecl> getptr();

  bool operator==(Decl &rhs) const override;

  bool operator!=(Decl &rhs) const override;

  bool operator==(const EnumTypeDecl &rhs) const;
  bool operator!=(const EnumTypeDecl &rhs) const;

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