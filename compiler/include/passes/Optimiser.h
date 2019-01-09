#pragma once

#ifndef ACC_OPTIMISER_H
#define ACC_OPTIMISER_H

#include <iostream>
#include <memory>
#include <set>
#include <vector>

#include "../AST.h"
#include "../ASTVisitor.h"

namespace ACC {

class Optimiser : public ASTVisitor<std::shared_ptr<ASTNode>> {

public:
  int optimisationsCount = 0;
  std::vector<std::string> optimisations;

  Optimiser(std::shared_ptr<Program> &progAST);

  void optimised(std::string error);

  void printOptimisations();

  void run();

private:
  int nodeCount = 0;
  std::ostream *output;
  std::shared_ptr<Program> &progAST;

  std::shared_ptr<Block> currScope;

  /* ---- Visit AST ---- */

  std::shared_ptr<ASTNode> visit(ArrayAccess &aa) override;
  std::shared_ptr<ASTNode> visit(ArrayType &at) override;
  std::shared_ptr<ASTNode> visit(Assign &as) override;
  std::shared_ptr<ASTNode> visit(BaseType &bt) override;
  std::shared_ptr<ASTNode> visit(BinOp &bo) override;
  std::shared_ptr<ASTNode> visit(Block &b) override;
  std::shared_ptr<ASTNode> visit(CharLiteral &cl) override;
  std::shared_ptr<ASTNode> visit(DoWhile &dw) override;
  std::shared_ptr<ASTNode> visit(EnumTypeDecl &etd) override;
  std::shared_ptr<ASTNode> visit(FieldAccess &fa) override;
  std::shared_ptr<ASTNode> visit(FunCall &fc) override;
  std::shared_ptr<ASTNode> visit(FunDecl &fd) override;
  std::shared_ptr<ASTNode> visit(FunDef &fd) override;
  std::shared_ptr<ASTNode> visit(If &i) override;
  std::shared_ptr<ASTNode> visit(IntLiteral &il) override;
  std::shared_ptr<ASTNode> visit(ParenthExpr &pe) override;
  std::shared_ptr<ASTNode> visit(PointerType &pt) override;
  std::shared_ptr<ASTNode> visit(Program &p) override;
  std::shared_ptr<ASTNode> visit(Return &r) override;
  std::shared_ptr<ASTNode> visit(SizeOf &so) override;
  std::shared_ptr<ASTNode> visit(StringLiteral &sl) override;
  std::shared_ptr<ASTNode> visit(StructType &st) override;
  std::shared_ptr<ASTNode> visit(StructTypeDecl &std) override;
  std::shared_ptr<ASTNode> visit(TypeCast &tc) override;
  std::shared_ptr<ASTNode> visit(TypeDefDecl &td) override;
  std::shared_ptr<ASTNode> visit(ValueAt &va) override;
  std::shared_ptr<ASTNode> visit(VarDecl &vd) override;
  std::shared_ptr<ASTNode> visit(VarExpr &ve) override;
  std::shared_ptr<ASTNode> visit(While &w) override;
};

}; // namespace ACC

#endif