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
  FunDecl(std::shared_ptr<Type> funType, std::string funName,
          std::vector<std::shared_ptr<VarDecl>> funArgs, Block funBlock) {
    // Boilerplate Code.
  }

  bool operator==(const FunDecl &fd) const { return true; }

  std::string toString() const override { return "FunDecl"; }
};

}; // namespace ACC

#endif