#pragma once

#ifndef ACC_ENUMCLASSTYPEDECL_H
#define ACC_ENUMCLASSTYPEDECL_H

#include <map>

#include "Decl.h"
#include "PrimitiveType.h"
#include "Type.h"

namespace ACC {

class EnumClassTypeDecl
    : public Decl,
      public Type,
      public atl::enable_shared_from_this<EnumClassTypeDecl> {

public:
  atl::string identifer;
  std::map<std::string, std::string> states;

  EnumClassTypeDecl(const atl::string &ident,
                    const std::map<std::string, std::string> &states);

  atl::shared_ptr<EnumClassTypeDecl> getptr();

  bool operator==(Decl &rhs) const override;
  bool operator!=(Decl &rhs) const override;

  atl::string getIdentifier() const override;

  int getBytes() const override;
  bool operator==(Type &rhs) const override;
  bool operator!=(Type &rhs) const override;

  bool operator==(const EnumClassTypeDecl &rhs) const;
  bool operator!=(const EnumClassTypeDecl &rhs) const;

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

}; // namespace ACC

#endif