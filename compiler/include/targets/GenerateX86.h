#pragma once

#ifndef ACC_GENERATEX86_H
#define ACC_GENERATEX86_H

#include "atl/include/string.h"
#include <algorithm>
#include <stack>

#include "../AST.h"
#include "../ASTVisitor.h"
#include "X86.h"

namespace ACC {

class GenerateX86 : public ASTVisitor<atl::shared_ptr<X86::Operand>> {

public:
  int errorCount = 0;
  atl::vector<atl::string> errors;

  GenerateX86(atl::shared_ptr<Program> progAST, const atl::string &outputFile);

  void error(atl::string error);

  void printErrors() const;

  void run();

private:
  X86::Writer x86;
  atl::shared_ptr<Program> progAST;

  atl::shared_ptr<Block> currScope;
  int blockCount = 0;
  int currFpOffset = 0;

  /* ---- X86 Memory ---- */

  void alloc(const VarDecl &vd);

  /* ---- Visit AST ---- */

  atl::shared_ptr<X86::Operand> visit(AddressOf &ao) override;
  atl::shared_ptr<X86::Operand> visit(Allocation &a) override;
  atl::shared_ptr<X86::Operand> visit(ArrayAccess &aa) override;
  atl::shared_ptr<X86::Operand> visit(ArrayType &at) override;
  atl::shared_ptr<X86::Operand> visit(Assign &as) override;
  atl::shared_ptr<X86::Operand> visit(BaseType &bt) override;
  atl::shared_ptr<X86::Operand> visit(BinOp &bo) override;
  atl::shared_ptr<X86::Operand> visit(Block &b) override;
  atl::shared_ptr<X86::Operand> visit(CharLiteral &cl) override;
  atl::shared_ptr<X86::Operand> visit(ClassType &ct) override;
  atl::shared_ptr<X86::Operand> visit(ClassTypeDecl &ctd) override;
  atl::shared_ptr<X86::Operand> visit(ConstructorDecl &cd) override;
  atl::shared_ptr<X86::Operand> visit(ConstructorDef &cd) override;
  atl::shared_ptr<X86::Operand> visit(DoWhile &dw) override;
  atl::shared_ptr<X86::Operand> visit(EnumTypeDecl &etd) override;
  atl::shared_ptr<X86::Operand> visit(For &f) override;
  atl::shared_ptr<X86::Operand> visit(FunCall &fc) override;
  atl::shared_ptr<X86::Operand> visit(FunDecl &fd) override;
  atl::shared_ptr<X86::Operand> visit(FunDef &fd) override;
  atl::shared_ptr<X86::Operand> visit(If &i) override;
  atl::shared_ptr<X86::Operand> visit(IntLiteral &il) override;
  atl::shared_ptr<X86::Operand> visit(MemberAccess &ma) override;
  atl::shared_ptr<X86::Operand> visit(MemberCall &mc) override;
  atl::shared_ptr<X86::Operand> visit(Namespace &n) override;
  atl::shared_ptr<X86::Operand> visit(ParenthExpr &pe) override;
  atl::shared_ptr<X86::Operand> visit(PointerType &pt) override;
  atl::shared_ptr<X86::Operand> visit(PrefixInc &pi) override;
  atl::shared_ptr<X86::Operand> visit(Program &p) override;
  atl::shared_ptr<X86::Operand> visit(ReferenceType &rt) override;
  atl::shared_ptr<X86::Operand> visit(Return &r) override;
  atl::shared_ptr<X86::Operand> visit(SizeOf &so) override;
  atl::shared_ptr<X86::Operand> visit(StringLiteral &sl) override;
  atl::shared_ptr<X86::Operand> visit(StructType &st) override;
  atl::shared_ptr<X86::Operand> visit(StructTypeDecl &std) override;
  atl::shared_ptr<X86::Operand> visit(TypeCast &tc) override;
  atl::shared_ptr<X86::Operand> visit(TypeDefDecl &td) override;
  atl::shared_ptr<X86::Operand> visit(ValueAt &va) override;
  atl::shared_ptr<X86::Operand> visit(VarDecl &vd) override;
  atl::shared_ptr<X86::Operand> visit(VarDef &vd) override;
  atl::shared_ptr<X86::Operand> visit(VarExpr &ve) override;
  atl::shared_ptr<X86::Operand> visit(While &w) override;

  /* ---- Helpers ---- */
  atl::shared_ptr<X86::Operand> genIntValue(int value);
};
}; // namespace ACC

#endif