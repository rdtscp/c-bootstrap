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

  std::shared_ptr<Block> funBlock;
  std::string funName;
  std::vector<std::shared_ptr<VarDecl>> funParams;
  std::shared_ptr<Type> funType;

public:
  FunDecl(std::shared_ptr<Block> funBlock, std::string funName,
          std::vector<std::shared_ptr<VarDecl>> funParams,
          std::shared_ptr<Type> funType)
      : funBlock(funBlock), funName(funName), funParams(funParams),
        funType(funType) {
    // Boilerplate Code.
  }

  bool operator==(const FunDecl &fd) const {
    /* Check Block */
    // TODO

    /* Check Name */
    if (fd.funName != funName)
      return false;

    /* Check Params */
    if (fd.funParams.size() != funParams.size())
      return false;
    for (int i=0; i < funParams.size(); i++) {
      VarDecl lhsParam = *funParams[i];
      VarDecl rhsParam = *fd.funParams[i];

      if (lhsParam != rhsParam)
        return false;
    }

    /* Check Type */
    if (*funType != *fd.funType)
      return false;

    return true;
  }

  std::string toString() const override { return "FunDecl"; }
};

}; // namespace ACC

#endif