#pragma once

#ifndef STRUCTTYPEDECL_H
#define STRUCTTYPEDECL_H

#include <vector>

#include "Decl.h"
#include "StructType.h"
#include "VarDecl.h"

namespace ACC {

class StructTypeDecl : public Decl {

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

#endif