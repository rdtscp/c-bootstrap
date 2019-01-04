#pragma once

#ifndef GENERATEX86_H
#define GENERATEX86_H

#include <algorithm>
#include <memory>
#include <set>
#include <stack>
#include <vector>

#include "../AST.h"
#include "../ASTVisitor.h"
#include "X86.h"

namespace ACC {

class GenerateX86 : public ASTVisitor<std::shared_ptr<X86::Operand>> {

public:
  int errorCount = 0;
  std::vector<std::string> errors;

  GenerateX86(std::shared_ptr<Program> progAST, std::string outputFile);

  void error(std::string error);

  void printErrors() const;

  void run();

private:
  X86::Writer x86;
  std::shared_ptr<Program> progAST;

  std::shared_ptr<Block> currScope;
  int blockCount = 0;
  int currFpOffset = 0;

  /* ---- X86 Memory ---- */

  void alloc(const VarDecl &vd);

  /* ---- Visit AST ---- */

  std::shared_ptr<X86::Operand> visit(ArrayAccess &aa) override;
  std::shared_ptr<X86::Operand> visit(ArrayType &at) override;
  std::shared_ptr<X86::Operand> visit(Assign &as) override;
  std::shared_ptr<X86::Operand> visit(BaseType &bt) override;
  std::shared_ptr<X86::Operand> visit(BinOp &bo) override;
  std::shared_ptr<X86::Operand> visit(Block &b) override;
  std::shared_ptr<X86::Operand> visit(CharLiteral &cl) override;
  std::shared_ptr<X86::Operand> visit(DoWhile &dw) override;
  std::shared_ptr<X86::Operand> visit(EnumType &et) override;
  std::shared_ptr<X86::Operand> visit(FieldAccess &fa) override;
  std::shared_ptr<X86::Operand> visit(FunCall &fc) override;
  std::shared_ptr<X86::Operand> visit(FunDecl &fd) override;
  std::shared_ptr<X86::Operand> visit(FunDef &fd) override;
  std::shared_ptr<X86::Operand> visit(If &i) override;
  std::shared_ptr<X86::Operand> visit(IntLiteral &il) override;
  std::shared_ptr<X86::Operand> visit(ParenthExpr &pe) override;
  std::shared_ptr<X86::Operand> visit(PointerType &pt) override;
  std::shared_ptr<X86::Operand> visit(Program &p) override;
  std::shared_ptr<X86::Operand> visit(Return &r) override;
  std::shared_ptr<X86::Operand> visit(SizeOf &so) override;
  std::shared_ptr<X86::Operand> visit(StringLiteral &sl) override;
  std::shared_ptr<X86::Operand> visit(StructType &st) override;
  std::shared_ptr<X86::Operand> visit(StructTypeDecl &std) override;
  std::shared_ptr<X86::Operand> visit(TypeCast &tc) override;
  std::shared_ptr<X86::Operand> visit(TypeDef &td) override;
  std::shared_ptr<X86::Operand> visit(ValueAt &va) override;
  std::shared_ptr<X86::Operand> visit(VarDecl &vd) override;
  std::shared_ptr<X86::Operand> visit(VarExpr &ve) override;
  std::shared_ptr<X86::Operand> visit(While &w) override;

  /* ---- Helpers ---- */
  std::shared_ptr<X86::Operand> genIntValue(int value);
};
}; // namespace ACC

#endif