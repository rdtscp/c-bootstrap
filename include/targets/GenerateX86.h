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

class GenerateX86 : public ASTVisitor<X86::Register> {

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

  X86::Register visit(ArrayAccess &aa) override;
  X86::Register visit(ArrayType &at) override;
  X86::Register visit(Assign &as) override;
  X86::Register visit(BaseType &bt) override;
  X86::Register visit(BinOp &bo) override;
  X86::Register visit(Block &b) override;
  X86::Register visit(CharLiteral &cl) override;
  X86::Register visit(FieldAccess &fa) override;
  X86::Register visit(FunCall &fc) override;
  X86::Register visit(FunDecl &fd) override;
  X86::Register visit(If &i) override;
  X86::Register visit(IntLiteral &il) override;
  X86::Register visit(ParenthExpr &pe) override;
  X86::Register visit(PointerType &pt) override;
  X86::Register visit(Program &p) override;
  X86::Register visit(Return &r) override;
  X86::Register visit(SizeOf &so) override;
  X86::Register visit(StringLiteral &sl) override;
  X86::Register visit(StructType &st) override;
  X86::Register visit(StructTypeDecl &std) override;
  X86::Register visit(TypeCast &tc) override;
  X86::Register visit(ValueAt &va) override;
  X86::Register visit(VarDecl &vd) override;
  X86::Register visit(VarExpr &ve) override;
  X86::Register visit(While &w) override;
};
}; // namespace ACC

#endif