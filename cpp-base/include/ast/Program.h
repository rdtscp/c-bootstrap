#pragma once

#ifndef PROGRAM_H
#define PROGRAM_H

#include "FunDecl.h"
#include "StructTypeDecl.h"
#include "VarDecl.h"

namespace ACC {

class Program {

public:
  std::vector<FunDecl> funDecls;
  std::vector<StructTypeDecl> structTypeDecls;
  std::vector<VarDecl> varDecls;

  Program(const std::vector<FunDecl> &funDecls,
          const std::vector<StructTypeDecl> &structTypeDecls,
          const std::vector<VarDecl> &varDecls)
      : funDecls(funDecls), structTypeDecls(structTypeDecls),
        varDecls(varDecls) {}
};

}; // namespace ACC

#endif