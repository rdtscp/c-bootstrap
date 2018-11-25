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

  std::shared_ptr<Block> getGlobalScope() {
    if (globalScope == nullptr)
      throw std::runtime_error(
          "Compiler Error: Attempted to get global scope before it was set.");
    return globalScope;
  }

  void setGlobalScope(const std::shared_ptr<Block> &block) {
    globalScope = block;
  }

  void accept(ASTVisitor<void> &v) override { return v.visit(*this); }
  std::string accept(ASTVisitor<std::string> &v) override {
    return v.visit(*this);
  }
  std::shared_ptr<Type> accept(ASTVisitor<std::shared_ptr<Type>> &v) override {
    return v.visit(*this);
  }
  std::string astClass() const override { return "Program"; }

private:
  std::shared_ptr<Block> globalScope = nullptr;
};

}; // namespace ACC

#endif