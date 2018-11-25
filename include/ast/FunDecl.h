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

  std::string accept(ASTVisitor &v) override { return v.visit(*this); }
  std::string astClass() const override { return "FunDecl"; }
};

}; // namespace ACC

#endif