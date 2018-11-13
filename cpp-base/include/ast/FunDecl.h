#pragma once

#ifndef FUNDECL_H
#define FUNDECL_H

#include "Type.h"
#include "VarDecl.h"

namespace ACC {

class FunDecl {

public:
  FunDecl(std::shared_ptr<Type> funType, std::string funName,
          std::vector<VarDecl> funArgs, Block funBlock) {
    // Boilerplate Code.
  }

  bool operator==(const FunDecl &fd) const { return true; }
};

}; // namespace ACC

#endif