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
  std::vector<std::shared_ptr<VarDecl>> varDecls;
  int allocSize;
  int compactSize;

  StructTypeDecl(std::shared_ptr<StructType> structType_ptr,
                 const std::vector<std::shared_ptr<VarDecl>> &varDecls)
      : varDecls(varDecls) {
    structType = structType_ptr;
  }

  std::string toString() const override {
    std::string output = structType->toString() + " {";
    for (const std::shared_ptr<VarDecl>& varDecl: varDecls)
      output += varDecl->toString();
    output += "}";
    return output;
  }

  std::string strVal() const override { return "StructTypeDecl"; }
};

}; // namespace ACC

#endif