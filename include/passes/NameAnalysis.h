#pragma once

#ifndef NAMEANALYSIS_H
#define NAMEANALYSIS_H

#include <iostream>
#include <memory>
#include <set>

#include "../AST.h"
#include "../ASTVisitor.h"

namespace ACC {

class NameAnalysis : public ASTVisitor {

public:
  int errorCount = 0;

  NameAnalysis(Program &progAST, std::ostream *out_stream = &std::cout)
      : progAST(progAST) {
    output = out_stream;
  }
  std::string error(std::string error) {
    errorCount++;
    *output << error << std::endl;
    return error;
  }
  void run() { visit(progAST); }

private:
  int nodeCount = 0;
  std::ostream *output;
  Program &progAST;

  std::shared_ptr<Block> currScope;
  bool createdFunctionScope = false;

  /* ---- Visit AST ---- */

  std::string visit(ArrayAccess &aa) override { return "undef"; }
  std::string visit(ArrayType &at) override { return "undef"; }
  std::string visit(Assign &as) override { return "undef"; }
  std::string visit(BaseType &bt) override { return "undef"; }
  std::string visit(BinOp &bo) override { return "undef"; }
  std::string visit(Block &b) override { return "undef"; }
  std::string visit(CharLiteral &cl) override { return "undef"; }
  std::string visit(FieldAccess &fa) override { return "undef"; }
  std::string visit(FunCall &fc) override { return "undef"; }
  std::string visit(FunDecl &fd) override {
    if (currScope->find(fd.getIdentifier()))
      return error("Attempted to declare a Function with an identifier that is "
                   "already in use: " +
                   fd.getIdentifier());
    currScope->insertDecl(std::make_shared<FunDecl>(fd));
    return "";
  }
  std::string visit(If &i) override { return "undef"; }
  std::string visit(IntLiteral &il) override { return "undef"; }
  std::string visit(ParenthExpr &pe) override { return "undef"; }
  std::string visit(PointerType &pt) override { return "undef"; }
  std::string visit(Program &p) override {
    currScope = std::make_shared<Block>(Block({}));
    for (const std::shared_ptr<Decl> &decl : p.decls) {
      decl->accept(*this);
    }
    return "";
  }
  std::string visit(Return &r) override { return "undef"; }
  std::string visit(SizeOf &so) override { return "undef"; }
  std::string visit(StringLiteral &sl) override { return "undef"; }
  std::string visit(StructType &st) override { return "undef"; }
  std::string visit(StructTypeDecl &std) override {
    if (currScope->find(std.getIdentifier()))
      return error("Attempted to declare a Struct with an identifier that is "
                   "already in use: " +
                   std.getIdentifier());

    currScope->insertDecl(std::make_shared<StructTypeDecl>(std));

    /* Check that the fields in this struct are unique */
    std::set<std::string> structTypeFields;
    for (const std::shared_ptr<VarDecl> field : std.varDecls) {
      if (structTypeFields.find(field->getIdentifier()) !=
          structTypeFields.end())
        return error("Struct " + std.getIdentifier() +
                     " contained multiple fields with the same identifier: " +
                     field->getIdentifier());
      structTypeFields.insert(field->identifer);
    }

    return "";
  }
  std::string visit(TypeCast &tc) override { return "undef"; }
  std::string visit(ValueAt &va) override { return "undef"; }
  std::string visit(VarDecl &vd) override {
    if (currScope->find(vd.getIdentifier()))
      return error("Attempted to declare a Variable with an identifier that is "
                   "already in use: " +
                   vd.getIdentifier());
    currScope->insertDecl(std::make_shared<VarDecl>(vd));
    return "";
  }
  std::string visit(VarExpr &ve) override { return "undef"; }
  std::string visit(While &w) override { return "undef"; }
};

}; // namespace ACC

#endif