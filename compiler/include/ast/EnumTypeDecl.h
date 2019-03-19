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
                     public atl::enable_shared_from_this<EnumTypeDecl> {

public:
  atl::shared_ptr<Identifier> identifier;
  std::map<std::string, std::string> states;

  EnumTypeDecl(const atl::shared_ptr<Identifier> &p_identifier,
               const std::map<std::string, std::string> &p_states);

  atl::shared_ptr<EnumTypeDecl> getptr();

  bool operator==(Decl &rhs) const override;
  bool operator!=(Decl &rhs) const override;

  atl::shared_ptr<Identifier> getIdentifier() const override;

  int getBytes() const override;
  bool operator==(Type &rhs) const override;
  bool operator!=(Type &rhs) const override;

  bool operator==(const EnumTypeDecl &rhs) const;
  bool operator!=(const EnumTypeDecl &rhs) const;

  void accept(ASTVisitor<void> &v) override;
  atl::string accept(ASTVisitor<atl::string> &v) override;

  atl::shared_ptr<Type> accept(ASTVisitor<atl::shared_ptr<Type>> &v) override;

  MIPS::Register accept(ASTVisitor<MIPS::Register> &v) override;
  atl::shared_ptr<X86::Operand>
  accept(ASTVisitor<atl::shared_ptr<X86::Operand>> &v) override;
  atl::shared_ptr<ASTNode>
  accept(ASTVisitor<atl::shared_ptr<ASTNode>> &v) override;
  atl::string astClass() const override;
};

} // namespace ACC

#endif