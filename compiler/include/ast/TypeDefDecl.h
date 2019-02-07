#pragma once

#ifndef ACC_TYPEDEF_H
#define ACC_TYPEDEF_H

#include "Decl.h"
#include "Type.h"

namespace ACC {

class TypeDefDecl : public Decl,
                    // public Type,
                    public std::enable_shared_from_this<TypeDefDecl> {

public:
  std::shared_ptr<Type> type;
  atl::string identifier;

  TypeDefDecl(std::shared_ptr<Type> type, const atl::string &identifier);

  std::shared_ptr<TypeDefDecl> getptr();

  bool operator==(Decl &rhs) const override;
  bool operator!=(Decl &rhs) const override;
  atl::string getIdentifier() const override;

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