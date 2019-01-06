#pragma once

#ifndef GENERATEMIPS_H
#define GENERATEMIPS_H

#include <algorithm>
#include <memory>
#include <set>
#include <stack>
#include <vector>

#include "../AST.h"
#include "../ASTVisitor.h"
#include "MIPS.h"

namespace ACC {

class GenerateMIPS : public ASTVisitor<MIPS::Register> {

public:
  int errorCount = 0;
  std::vector<std::string> errors;

  GenerateMIPS(std::shared_ptr<Program> progAST, std::string outputFile);

  void error(std::string error);

  void printErrors() const;

  void run();

private:
  MIPS::Writer MIPS;
  std::shared_ptr<Program> progAST;

  std::shared_ptr<Block> currScope;
  int currFpOffset = 0;

  /* ---- Register Management ---- */

  std::stack<MIPS::Register> freeArgsRegs;
  std::stack<MIPS::Register> freeSaveRegs;
  std::stack<MIPS::Register> freeTempRegs;

  void freeAllRegs();

  void freeAllArgsRegs();

  void freeAllSaveRegs();

  void freeAllTempRegs();

  void freeRegister(MIPS::Register reg);

  MIPS::Register getArgsRegister();

  MIPS::Register getSaveRegister();

  MIPS::Register getTempRegister();

  /* ---- MIPS Memory ---- */

  void alloc(const int bytes);

  void stackPush(const MIPS::Register &src);

  void stackPop(const MIPS::Register &dest);

  /* ---- Visit AST ---- */

  MIPS::Register visit(ArrayAccess &aa) override;
  MIPS::Register visit(ArrayType &at) override;
  MIPS::Register visit(Assign &as) override;
  MIPS::Register visit(BaseType &bt) override;
  MIPS::Register visit(BinOp &bo) override;
  MIPS::Register visit(Block &b) override;
  MIPS::Register visit(CharLiteral &cl) override;
  MIPS::Register visit(DoWhile &dw) override;
  MIPS::Register visit(EnumTypeDecl &etd) override;
  MIPS::Register visit(FieldAccess &fa) override;
  MIPS::Register visit(FunCall &fc) override;
  MIPS::Register visit(FunDecl &fd) override;
  MIPS::Register visit(FunDef &fd) override;
  MIPS::Register visit(If &i) override;
  MIPS::Register visit(IntLiteral &il) override;
  MIPS::Register visit(ParenthExpr &pe) override;
  MIPS::Register visit(PointerType &pt) override;
  MIPS::Register visit(Program &p) override;
  MIPS::Register visit(Return &r) override;
  MIPS::Register visit(SizeOf &so) override;
  MIPS::Register visit(StringLiteral &sl) override;
  MIPS::Register visit(StructType &st) override;
  MIPS::Register visit(StructTypeDecl &std) override;
  MIPS::Register visit(TypeCast &tc) override;
  MIPS::Register visit(TypeDefDecl &td) override;
  MIPS::Register visit(ValueAt &va) override;
  MIPS::Register visit(VarDecl &vd) override;
  MIPS::Register visit(VarExpr &ve) override;
  MIPS::Register visit(While &w) override;
};
}; // namespace ACC

#endif