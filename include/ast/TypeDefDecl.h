#pragma once

#ifndef TYPEDEF_H
#define TYPEDEF_H

#include <string>

#include "Decl.h"
#include "Type.h"

namespace ACC {

class TypeDefDecl : public Decl,
                public Type,
                public std::enable_shared_from_this<TypeDefDecl> {

public:
  std::shared_ptr<Type> type;
  std::string identifier;

  TypeDefDecl(std::shared_ptr<Type> type, std::string identifier);

  std::shared_ptr<TypeDefDecl::Decl> getptr();

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