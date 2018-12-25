#pragma once

#ifndef TYPEANALYSIS_H
#define TYPEANALYSIS_H

#include <iostream>
#include <memory>
#include <set>
#include <vector>

#include "../ASTVisitor.h"
#include "../AST.h"

namespace ACC {

class TypeAnalysis : public ASTVisitor<std::shared_ptr<Type>> {

public:
  int errorCount = 0;
  std::vector<std::string> errors;

  TypeAnalysis(std::shared_ptr<Program> progAST);

  std::shared_ptr<Type> error(std::string error);

  void printErrors();

  void run();

private:
  int nodeCount = 0;
  std::ostream *output;
  std::shared_ptr<Program> progAST;

  std::shared_ptr<Block> currScope;

  /* ---- Visit AST ---- */

  std::shared_ptr<Type> visit(ArrayAccess &aa) override;
  std::shared_ptr<Type> visit(ArrayType &at) override;
  std::shared_ptr<Type> visit(Assign &as) override;
  std::shared_ptr<Type> visit(BaseType &bt) override;
  std::shared_ptr<Type> visit(BinOp &bo) override;
  std::shared_ptr<Type> visit(Block &b) override;
  std::shared_ptr<Type> visit(CharLiteral &cl) override;
  std::shared_ptr<Type> visit(FieldAccess &fa) override;
  std::shared_ptr<Type> visit(FunCall &fc) override;
  std::shared_ptr<Type> visit(FunDecl &fd) override;
  std::shared_ptr<Type> visit(If &i) override;
  std::shared_ptr<Type> visit(IntLiteral &il) override;
  std::shared_ptr<Type> visit(ParenthExpr &pe) override;
  std::shared_ptr<Type> visit(PointerType &pt) override;
  std::shared_ptr<Type> visit(Program &p) override;
  std::shared_ptr<Type> visit(Return &r) override;
  std::shared_ptr<Type> visit(SizeOf &so) override;
  std::shared_ptr<Type> visit(StringLiteral &sl) override;
  std::shared_ptr<Type> visit(StructType &st) override;
  std::shared_ptr<Type> visit(StructTypeDecl &std) override;
  std::shared_ptr<Type> visit(TypeCast &tc) override;
  std::shared_ptr<Type> visit(ValueAt &va) override;
  std::shared_ptr<Type> visit(VarDecl &vd) override;
  std::shared_ptr<Type> visit(VarExpr &ve) override;
  std::shared_ptr<Type> visit(While &w) override;
};

}; // namespace ACC

#endif