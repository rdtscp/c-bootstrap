#pragma once

#include <vector>

#include "StructType.h"
#include "VarDecl.h"

namespace ACC {

class StructTypeDecl {

public:
  std::shared_ptr<StructType> structType;
  std::vector<VarDecl> varDecls;
  int allocSize;
  int compactSize;

  StructTypeDecl(std::shared_ptr<StructType> structType_ptr, const std::vector<VarDecl>& varDecls) : varDecls(varDecls) {
    structType = structType_ptr;
  }
};

}; // namespace ACC