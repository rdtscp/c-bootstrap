#pragma once

#ifndef STRUCTTYPEDECL_H
#define STRUCTTYPEDECL_H

#include <vector>

#include "Decl.h"
#include "StructType.h"
#include "VarDecl.h"

namespace ACC {

class StructTypeDecl : public Decl,
                       public std::enable_shared_from_this<StructTypeDecl> {

public:
  std::shared_ptr<StructType> structType;
  std::vector<std::shared_ptr<VarDecl>> varDecls;
  int allocSize;
  int compactSize;

  StructTypeDecl(std::shared_ptr<StructType> structType,
                 const std::vector<std::shared_ptr<VarDecl>> &varDecls);

  std::shared_ptr<StructTypeDecl> getptr();

  bool operator==(Decl &rhs) const override;
  bool operator!=(Decl &rhs) const override;

  bool operator==(const StructTypeDecl &rhs) const;
  bool operator!=(const StructTypeDecl &rhs) const;

  std::string getIdentifier() const override;

  void accept(ASTVisitor<void> &v) override;
  std::string accept(ASTVisitor<std::string> &v) override;
  std::shared_ptr<Type> accept(ASTVisitor<std::shared_ptr<Type>> &v) override;
  MIPS::Register accept(ASTVisitor<MIPS::Register> &v) override;
  std::shared_ptr<X86::Operand> accept(ASTVisitor<std::shared_ptr<X86::Operand>> &v) override;
  std::string astClass() const override;
};

}; // namespace ACC

#endif