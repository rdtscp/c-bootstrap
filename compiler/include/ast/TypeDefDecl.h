#pragma once

#ifndef ACC_TYPEDEF_H
#define ACC_TYPEDEF_H

#include "Decl.h"
#include "Type.h"

namespace ACC {

class TypeDefDecl : public Decl,
                    // public Type,
                    public atl::enable_shared_from_this<TypeDefDecl> {

public:
  atl::shared_ptr<Type> type;
  atl::shared_ptr<Identifier> identifier;

  TypeDefDecl(const atl::shared_ptr<Type> &type,
              const atl::shared_ptr<Identifier> &identifier);

  atl::shared_ptr<TypeDefDecl> getptr();

  bool operator==(Decl &rhs) const override;
  bool operator!=(Decl &rhs) const override;
  atl::shared_ptr<Identifier> getIdentifier() const override;

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