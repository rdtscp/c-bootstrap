#pragma once

#ifndef ACC_ENUMTYPEDECL_H
#define ACC_ENUMTYPEDECL_H

#include <map>

#include "Decl.h"
#include "PrimitiveType.h"
#include "Type.h"

namespace ACC {

class EnumTypeDecl : public Decl,
                     public Type,
                     public std::enable_shared_from_this<EnumTypeDecl> {

public:
  atl::string identifer;
  std::map<std::string, std::string> states;

  EnumTypeDecl(const atl::string &ident,
               std::map<std::string, std::string> states);

  std::shared_ptr<EnumTypeDecl> getptr();

  bool operator==(Decl &rhs) const override;
  bool operator!=(Decl &rhs) const override;

  atl::string getIdentifier() const override;

  int getBytes() const override;
  bool operator==(Type &rhs) const override;
  bool operator!=(Type &rhs) const override;

  bool operator==(const EnumTypeDecl &rhs) const;
  bool operator!=(const EnumTypeDecl &rhs) const;

  void accept(ASTVisitor<void> &v) override;
  atl::string accept(ASTVisitor<atl::string> &v) override;

  std::shared_ptr<Type> accept(ASTVisitor<std::shared_ptr<Type>> &v) override;

  MIPS::Register accept(ASTVisitor<MIPS::Register> &v) override;
  std::shared_ptr<X86::Operand>
  accept(ASTVisitor<std::shared_ptr<X86::Operand>> &v) override;
  std::shared_ptr<ASTNode>
  accept(ASTVisitor<std::shared_ptr<ASTNode>> &v) override;
  atl::string astClass() const override;
};

}; // namespace ACC

#endif