#pragma once

#ifndef PROGRAM_H
#define PROGRAM_H

#include "FunDecl.h"
#include "StructTypeDecl.h"
#include "VarDecl.h"

namespace ACC {

class Program {

public:
  std::vector<std::shared_ptr<Decl>> decls;
  std::vector<FunDecl> funDecls;
  std::vector<StructTypeDecl> structTypeDecls;
  std::vector<VarDecl> varDecls;

  Program(const std::vector<FunDecl> &funDecls,
          const std::vector<StructTypeDecl> &structTypeDecls,
          const std::vector<VarDecl> &varDecls)
      : funDecls(funDecls), structTypeDecls(structTypeDecls),
        varDecls(varDecls) {}

  Program(const std::vector<std::shared_ptr<Decl>> &decls) : decls(decls) {
    for (const std::shared_ptr<Decl> decl : decls) {
      if (decl->toString() == "FunDecl")
        funDecls.push_back(*static_cast<FunDecl *>(decl.get()));
      if (decl->toString() == "VarDecl")
        varDecls.push_back(*static_cast<VarDecl *>(decl.get()));
      if (decl->toString() == "StructTypeDecl")
        structTypeDecls.push_back(*static_cast<StructTypeDecl *>(decl.get()));
    }
  }

  std::string toString() const {
    std::string output;
    for (const std::shared_ptr<Decl> &decl : decls)
      output += decl->toString();
    return output;
  }
};

}; // namespace ACC

#endif