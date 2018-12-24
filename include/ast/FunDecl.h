#pragma once

#ifndef FUNDECL_H
#define FUNDECL_H

#include <vector>

#include "Block.h"
#include "Decl.h"
#include "Type.h"
#include "VarDecl.h"

namespace ACC {

class FunDecl : public Decl {

public:
  std::shared_ptr<Block> funBlock;
  std::string funName;
  std::vector<std::shared_ptr<VarDecl>> funParams;
  std::shared_ptr<Type> funType;

  FunDecl(std::shared_ptr<Block> funBlock, std::string funName,
          std::vector<std::shared_ptr<VarDecl>> funParams,
          std::shared_ptr<Type> funType)
      : funBlock(funBlock), funName(funName), funParams(funParams),
        funType(funType) {}

  bool operator==(Decl &rhs) const override {
    if (rhs.astClass() == astClass())
      return *this == *static_cast<FunDecl *>(&rhs);
    return false;
  }
  bool operator!=(Decl &rhs) const override { return !(*this == rhs); }

  bool operator==(const FunDecl &rhs) const {
    if (*funType != *rhs.funType)
      return false;

    if (funName != rhs.funName)
      return false;

    if (funParams.size() != rhs.funParams.size())
      return false;

    for (int i = 0; i < funParams.size(); i++)
      if (*funParams[i] != *rhs.funParams[i])
        return false;

    /* Check Block*/

    return true;
  }
  bool operator!=(const FunDecl &rhs) const { return !(*this == rhs); }

  std::string getIdentifier() const override { return funName; }

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
  std::string astClass() const override { return "FunDecl"; }
};

}; // namespace ACC

#endif