#pragma once

#include "FunDecl.h"
#include "StructTypeDecl.h"
#include "VarDecl.h"

namespace ACC {

class Program {

public:
  std::vector<ACC::FunDecl> funDecls;
  std::vector<ACC::StructTypeDecl> structTypeDecls;
  std::vector<ACC::VarDecl> varDecls;

  Program(const std::vector<ACC::FunDecl> &funDecls,
          const std::vector<ACC::StructTypeDecl> &structTypeDecls,
          const std::vector<ACC::VarDecl> &varDecls)
      : funDecls(funDecls), structTypeDecls(structTypeDecls),
        varDecls(varDecls) {}
};

}; // namespace ACC
