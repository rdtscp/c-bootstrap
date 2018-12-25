#pragma once

#ifndef DOTGRAPH_H
#define DOTGRAPH_H

#include <iostream>

#include "../AST.h"
#include "../ASTVisitor.h"

namespace ACC {

class DotGraph : public ASTVisitor<std::string> {

public:
  DotGraph(Program &progAST);
  void print();

private:
  int nodeCount = 0;
  Program &progAST;

  void declare(const std::string &nodeID, const std::string &label);

  void join(const std::string &lhs, const std::string &rhs);

  void put(const std::string &str);

  /* ---- Visit AST ---- */

  std::string visit(ArrayAccess &aa) override;
  std::string visit(ArrayType &at) override;
  std::string visit(Assign &as) override;
  std::string visit(BaseType &bt) override;
  std::string visit(BinOp &bo) override;
  std::string visit(Block &b) override;
  std::string visit(CharLiteral &cl) override;
  std::string visit(FieldAccess &fa) override;
  std::string visit(FunCall &fc) override;
  std::string visit(FunDecl &fd) override;
  std::string visit(If &i) override;
  std::string visit(IntLiteral &il) override;
  std::string visit(ParenthExpr &pe) override;
  std::string visit(PointerType &pt) override;
  std::string visit(Program &p) override;
  std::string visit(Return &r) override;
  std::string visit(SizeOf &so) override;
  std::string visit(StringLiteral &sl) override;
  std::string visit(StructType &st) override;
  std::string visit(StructTypeDecl &std) override;
  std::string visit(TypeCast &tc) override;
  std::string visit(ValueAt &va) override;
  std::string visit(VarDecl &vd) override;
  std::string visit(VarExpr &ve) override;
  std::string visit(While &w) override;
};

}; // namespace ACC

#endif