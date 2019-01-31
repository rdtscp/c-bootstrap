#pragma once

#ifndef ACC_DOTGRAPH_H
#define ACC_DOTGRAPH_H

#include "atl/include/string.h"
#include <iostream>

#include "../AST.h"
#include "../ASTVisitor.h"

namespace ACC {

class DotGraph : public ASTVisitor<atl::string> {

public:
  DotGraph(std::shared_ptr<Program> progAST);
  void print();

private:
  int nodeCount = 0;
  std::shared_ptr<Program> progAST;

  void declare(const atl::string &nodeID, const atl::string &label);

  void join(const atl::string &lhs, const atl::string &rhs);

  void put(const atl::string &str);

  /* ---- Visit AST ---- */

  atl::string visit(ArrayAccess &aa) override;
  atl::string visit(ArrayType &at) override;
  atl::string visit(Assign &as) override;
  atl::string visit(BaseType &bt) override;
  atl::string visit(BinOp &bo) override;
  atl::string visit(Block &b) override;
  atl::string visit(CharLiteral &cl) override;
  atl::string visit(DoWhile &dw) override;
  atl::string visit(EnumTypeDecl &etd) override;
  atl::string visit(FieldAccess &fa) override;
  atl::string visit(FunCall &fc) override;
  atl::string visit(FunDecl &fd) override;
  atl::string visit(FunDef &fd) override;
  atl::string visit(If &i) override;
  atl::string visit(IntLiteral &il) override;
  atl::string visit(Namespace &n) override;
  atl::string visit(ParenthExpr &pe) override;
  atl::string visit(PointerType &pt) override;
  atl::string visit(Program &p) override;
  atl::string visit(Return &r) override;
  atl::string visit(SizeOf &so) override;
  atl::string visit(StringLiteral &sl) override;
  atl::string visit(StructType &st) override;
  atl::string visit(StructTypeDecl &std) override;
  atl::string visit(TypeCast &tc) override;
  atl::string visit(TypeDefDecl &td) override;
  atl::string visit(ValueAt &va) override;
  atl::string visit(VarDecl &vd) override;
  atl::string visit(VarExpr &ve) override;
  atl::string visit(While &w) override;
};

}; // namespace ACC

#endif