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
  std::shared_ptr<Block> globalScope = nullptr;

  Program(const std::vector<std::shared_ptr<Decl>> &decls) : decls(decls) {}

  void accept(ASTVisitor<void> &v) override { return v.visit(*this); }
  std::string accept(ASTVisitor<std::string> &v) override {
    return v.visit(*this);
  }
  std::shared_ptr<Type> accept(ASTVisitor<std::shared_ptr<Type>> &v) override {
    return v.visit(*this);
  }
  MIPS::Register accept(ASTVisitor<MIPS::Register> &v) override {
    return v.visit(*this);
  }
  std::string astClass() const override { return "Program"; }
};

}; // namespace ACC

#endif