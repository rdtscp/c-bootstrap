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
  int numBytes;
  std::shared_ptr<FunDecl> parentFunc;
  int fpOffset;
  int spOffset;

  VarDecl(std::shared_ptr<Type> type, std::string p_identifer)
      : identifer(p_identifer), numBytes(0), fpOffset(0), spOffset(0),
        type(type) {}

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
  std::string astClass() const override { return "VarDecl"; }
};

}; // namespace ACC

#endif