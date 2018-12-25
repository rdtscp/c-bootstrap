#pragma once

#ifndef VARDECL_H
#define VARDECL_H

#include <memory>

#include "Decl.h"

namespace ACC {

class FunDecl;

class VarDecl : public Decl, public Stmt {

public:
  std::shared_ptr<Type> type;
  std::string identifer;
  std::shared_ptr<FunDecl> parentFunc;

  int fpOffset = 0;

  VarDecl(std::shared_ptr<Type> type, std::string p_identifer)
      : identifer(p_identifer), type(type) {}

  bool operator==(Decl &rhs) const override {
    if (rhs.astClass() == astClass())
      return *this == *static_cast<VarDecl *>(&rhs);
    return false;
  }
  bool operator!=(Decl &rhs) const override { return !(*this == rhs); }

  bool operator==(const VarDecl &rhs) const {
    if (*type != *rhs.type)
      return false;

    if (identifer != rhs.identifer)
      return false;

    return true;
  }
  bool operator!=(const VarDecl &rhs) const { return !(*this == rhs); }

  int getBytes() const { return type->getBytes(); }

  std::string getIdentifier() const override { return identifer; }

  void accept(ASTVisitor<void> &v) override { return v.visit(*this); }
  std::string accept(ASTVisitor<std::string> &v) override {
    return v.visit(*this);
  }
  std::shared_ptr<Type> accept(ASTVisitor<std::shared_ptr<Type>> &v) override {
    return v.visit(*this);
  }
  MIPS::Register accept(ASTVisitor<MIPS::Register> &v) override {
    return v.visit(*this);
  }
  X86::Register accept(ASTVisitor<X86::Register> &v) override {
    return v.visit(*this);
  }
  std::string astClass() const override { return "VarDecl"; }
};

}; // namespace ACC

#endif