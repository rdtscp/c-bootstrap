#pragma once

#ifndef ACC_OPTIMISER_H
#define ACC_OPTIMISER_H

#include "../AST.h"
#include "../ASTVisitor.h"

namespace ACC {

class Optimiser : public ASTVisitor<atl::shared_ptr<ASTNode>> {

public:
  int optimisationsCount = 0;
  atl::vector<atl::string> optimisations;

  Optimiser(atl::shared_ptr<Program> &progAST);

  void optimised(const atl::string &error);

  void printOptimisations();

  void run();

private:
  int nodeCount = 0;
  std::ostream *output;
  atl::shared_ptr<Program> &progAST;

  atl::shared_ptr<Block> currScope;

  /* ---- Visit AST ---- */

  atl::shared_ptr<ASTNode> visit(ArrayAccess &aa) override;
  atl::shared_ptr<ASTNode> visit(ArrayType &at) override;
  atl::shared_ptr<ASTNode> visit(Assign &as) override;
  atl::shared_ptr<ASTNode> visit(BaseType &bt) override;
  atl::shared_ptr<ASTNode> visit(BinOp &bo) override;
  atl::shared_ptr<ASTNode> visit(Block &b) override;
  atl::shared_ptr<ASTNode> visit(CharLiteral &cl) override;
  atl::shared_ptr<ASTNode> visit(ClassType &ct) override;
  atl::shared_ptr<ASTNode> visit(ClassTypeDecl &ctd) override;
  atl::shared_ptr<ASTNode> visit(DoWhile &dw) override;
  atl::shared_ptr<ASTNode> visit(EnumTypeDecl &etd) override;
  atl::shared_ptr<ASTNode> visit(FieldAccess &fa) override;
  atl::shared_ptr<ASTNode> visit(FunCall &fc) override;
  atl::shared_ptr<ASTNode> visit(FunDecl &fd) override;
  atl::shared_ptr<ASTNode> visit(FunDef &fd) override;
  atl::shared_ptr<ASTNode> visit(If &i) override;
  atl::shared_ptr<ASTNode> visit(IntLiteral &il) override;
  atl::shared_ptr<ASTNode> visit(Namespace &n) override;
  atl::shared_ptr<ASTNode> visit(ParenthExpr &pe) override;
  atl::shared_ptr<ASTNode> visit(PointerType &pt) override;
  atl::shared_ptr<ASTNode> visit(Program &p) override;
  atl::shared_ptr<ASTNode> visit(Return &r) override;
  atl::shared_ptr<ASTNode> visit(SizeOf &so) override;
  atl::shared_ptr<ASTNode> visit(StringLiteral &sl) override;
  atl::shared_ptr<ASTNode> visit(StructType &st) override;
  atl::shared_ptr<ASTNode> visit(StructTypeDecl &std) override;
  atl::shared_ptr<ASTNode> visit(TypeCast &tc) override;
  atl::shared_ptr<ASTNode> visit(TypeDefDecl &td) override;
  atl::shared_ptr<ASTNode> visit(ValueAt &va) override;
  atl::shared_ptr<ASTNode> visit(VarDecl &vd) override;
  atl::shared_ptr<ASTNode> visit(VarExpr &ve) override;
  atl::shared_ptr<ASTNode> visit(While &w) override;
};

}; // namespace ACC

#endif