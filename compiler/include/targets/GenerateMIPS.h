#pragma once

#ifndef ACC_GENERATEMIPS_H
#define ACC_GENERATEMIPS_H

#include "atl/include/string.h"
#include <algorithm>
#include <stack>

#include "../AST.h"
#include "../ASTVisitor.h"
#include "MIPS.h"

namespace ACC {

class GenerateMIPS : public ASTVisitor<MIPS::Register> {

public:
  int errorCount = 0;
  atl::vector<atl::string> errors;

  GenerateMIPS(atl::shared_ptr<Program> progAST, const atl::string &outputFile);

  void error(const atl::string &error);

  void printErrors() const;

  void run();

private:
  MIPS::Writer MIPS;
  atl::shared_ptr<Program> progAST;

  atl::shared_ptr<Block> currScope;
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

  MIPS::Register visit(AddressOf &ao) override;
  MIPS::Register visit(Allocation &a) override;
  MIPS::Register visit(ArrayAccess &aa) override;
  MIPS::Register visit(ArrayType &at) override;
  MIPS::Register visit(Assign &as) override;
  MIPS::Register visit(BaseType &bt) override;
  MIPS::Register visit(BinOp &bo) override;
  MIPS::Register visit(Block &b) override;
  MIPS::Register visit(CharLiteral &cl) override;
  MIPS::Register visit(ClassType &ct) override;
  MIPS::Register visit(ClassTypeDecl &ctd) override;
  MIPS::Register visit(ConstructorDecl &cd) override;
  MIPS::Register visit(ConstructorDef &cd) override;
  MIPS::Register visit(DestructorDecl &dd) override;
  MIPS::Register visit(DestructorDef &dd) override;
  MIPS::Register visit(DoWhile &dw) override;
  MIPS::Register visit(EnumTypeDecl &etd) override;
  MIPS::Register visit(For &f) override;
  MIPS::Register visit(FunCall &fc) override;
  MIPS::Register visit(FunDecl &fd) override;
  MIPS::Register visit(FunDef &fd) override;
  MIPS::Register visit(If &i) override;
  MIPS::Register visit(IntLiteral &il) override;
  MIPS::Register visit(MemberAccess &ma) override;
  MIPS::Register visit(MemberCall &mc) override;
  MIPS::Register visit(Namespace &n) override;
  MIPS::Register visit(ParenthExpr &pe) override;
  MIPS::Register visit(PointerType &pt) override;
  MIPS::Register visit(PrefixInc &pi) override;
  MIPS::Register visit(Program &p) override;
  MIPS::Register visit(ReferenceType &rt) override;
  MIPS::Register visit(Return &r) override;
  MIPS::Register visit(SizeOf &so) override;
  MIPS::Register visit(StringLiteral &sl) override;
  MIPS::Register visit(StructType &st) override;
  MIPS::Register visit(StructTypeDecl &std) override;
  MIPS::Register visit(TypeCast &tc) override;
  MIPS::Register visit(TypeDefDecl &td) override;
  MIPS::Register visit(ValueAt &va) override;
  MIPS::Register visit(VarDecl &vd) override;
  MIPS::Register visit(VarDef &vd) override;
  MIPS::Register visit(VarExpr &ve) override;
  MIPS::Register visit(While &w) override;
};
}; // namespace ACC

#endif