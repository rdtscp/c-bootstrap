#pragma once

#ifndef PROGRAM_H
#define PROGRAM_H

#include "../ASTNode.h"

#include "FunDecl.h"
#include "StructTypeDecl.h"
#include "VarDecl.h"

namespace ACC {

class Program : public ASTNode {

public:
  std::vector<std::shared_ptr<Decl>> decls;

  Program(const std::vector<std::shared_ptr<Decl>> &decls) : decls(decls) {}

  void accept(ASTVisitor &v) override { v.visit(*this); }
  std::string astClass() const override { return "Program"; }
};

}; // namespace ACC

#endif