#pragma once

#ifndef NAMEANALYSIS_H
#define NAMEANALYSIS_H

#include <iostream>
#include <memory>
#include <set>
#include <vector>

#include "../ASTVisitor.h"
#include "../AST.h"

namespace ACC {

class NameAnalysis : public ASTVisitor<void> {

public:
  int errorCount = 0;
  std::vector<std::string> errors;

  NameAnalysis(std::shared_ptr<Program> progAST);

  void error(std::string error);

  void printErrors();

  void run();

private:
  int nodeCount = 0;
  std::ostream *output;
  std::shared_ptr<Program> progAST;

  std::shared_ptr<Block> currScope;

  /* ---- Visit AST ---- */

  void visit(ArrayAccess &aa) override;
  void visit(ArrayType &at) override;
  void visit(Assign &as) override;
  void visit(BaseType &bt) override;
  void visit(BinOp &bo) override;
  void visit(Block &b) override;
  void visit(CharLiteral &cl) override;
  void visit(FieldAccess &fa) override;
  void visit(FunCall &fc) override;
  void visit(FunDecl &fd) override;
  void visit(If &i) override;
  void visit(IntLiteral &il) override;
  void visit(ParenthExpr &pe) override;
  void visit(PointerType &pt) override;
  void visit(Program &p) override;
  void visit(Return &r) override;
  void visit(SizeOf &so) override;
  void visit(StringLiteral &sl) override;
  void visit(StructType &st) override;
  void visit(StructTypeDecl &std) override;
  void visit(TypeCast &tc) override;
  void visit(ValueAt &va) override;
  void visit(VarDecl &vd) override;
  void visit(VarExpr &ve) override;
  void visit(While &w) override;
};

}; // namespace ACC

#endif