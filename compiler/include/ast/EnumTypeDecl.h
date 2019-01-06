#pragma once

#ifndef ENUMTYPEDECL_H
#define ENUMTYPEDECL_H

#include <map>
#include <string>

#include "Decl.h"
#include "PrimitiveType.h"
#include "Type.h"

namespace ACC {

class EnumTypeDecl : public Decl,
                     public Type,
                     public std::enable_shared_from_this<EnumTypeDecl> {

public:
  std::string identifer;
  std::map<std::string, std::string> states;

  EnumTypeDecl(std::string ident, std::map<std::string, std::string> states);

  std::shared_ptr<EnumTypeDecl::Decl> getptr();

  bool operator==(Decl &rhs) const override;
  bool operator!=(Decl &rhs) const override;

  std::string getIdentifier() const override;

  int getBytes() const override;
  bool operator==(Type &rhs) const override;
  bool operator!=(Type &rhs) const override;

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