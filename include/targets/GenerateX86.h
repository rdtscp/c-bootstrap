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

  GenerateX86(std::shared_ptr<Program> progAST, std::string outputFile) : progAST(progAST) {
    X86::x86Output.open(outputFile);
  }

  void error(std::string error) {
    errorCount++;
    errors.push_back(error);
  }

  void printErrors() const {
    std::cerr << "FATAL x86 Generation Errors:" << std::endl;
    for (const auto &error : errors)
      std::cerr << "\t" << error << std::endl;
  }

  void run() {
    visit(*progAST);
    X86::x86Output.close();
  }

private:
  std::shared_ptr<Program> progAST;

  std::shared_ptr<Block> currScope;
  int blockCount = 0;
  int currFpOffset = 0;

  /* ---- Register Management ---- */

  /* ---- X86 Memory ---- */

  void alloc(const VarDecl &vd) {
    const int size = vd.getBytes();

    std::string allocSize = "DB ?";
    if (size > 1)
      allocSize = "DW ?";
    if (size > 2)
      allocSize = "DD ?";
    if (size > 4)
      allocSize = "DB " + std::to_string(size) + "DUP(?)";

    X86::write(vd.getIdentifier() + " " + allocSize);
  }

  /* ---- Visit AST ---- */

  X86::Register visit(ArrayAccess &aa) override {
    aa.array->accept(*this);
    return X86::Register();
  }
  X86::Register visit(ArrayType &at) override { return X86::Register(); }
  X86::Register visit(Assign &as) override {
    X86::Register lhsRegRes = as.lhs->accept(*this);
    X86::Register rhsResReg = as.rhs->accept(*this);
    X86::mov(lhsRegRes, rhsResReg);
    return X86::Register();
  }
  X86::Register visit(BaseType &bt) override { return X86::Register(); }
  X86::Register visit(BinOp &bo) override {
    bo.lhs->accept(*this);
    bo.rhs->accept(*this);
    return X86::Register();
  }
  X86::Register visit(Block &b) override {
    currScope = std::make_shared<Block>(b);
    for (const auto &stmt : b.blockStmts)
      stmt->accept(*this);

    // /* Clean up the stack. */
    // for (const auto &ident_decl : b.blockDecls) {
    //   std::shared_ptr<Decl> currDecl = ident_decl.second;
    //   if (currDecl->astClass() == "VarDecl") {
    //     /* ---- Deconstruct VarDecl ---- */
    //   }
    // }
    currScope = b.outerBlock;
    return X86::Register();
  }
  X86::Register visit(CharLiteral &cl) override { return X86::Register(); }
  X86::Register visit(FieldAccess &fa) override {
    fa.object->accept(*this);
    return X86::Register();
  }
  X86::Register visit(FunCall &fc) override {
    std::vector<std::shared_ptr<Expr>> revArgs = fc.funArgs;
    std::reverse(std::begin(revArgs), std::end(revArgs));
    for (const auto &arg : revArgs) {
      X86::Register argReg = arg->accept(*this);
      X86::push(argReg);
    }
    X86::call(fc.funName);
    return X86::eax;
  }
  X86::Register visit(FunDecl &fd) override {
    currScope = fd.funBlock;

    X86::block(fd.getIdentifier() + "FunDecl");

    /* ---- Callee Prologue ---- */
    X86::push(X86::ebp);
    X86::mov(X86::ebp, X86::esp);
    X86::push(X86::ebx);
    X86::push(X86::edi);
    X86::push(X86::esi);

    /* ---- Execute Function ---- */
    X86::comment(" ---- Function Body ----");
    fd.funBlock->accept(*this);
    X86::comment(" -----------------------");

    /* -------------------------- */

    /* ---- Callee Epilogue ---- */
    X86::pop(X86::esi);
    X86::pop(X86::edi);
    X86::pop(X86::ebx);
    X86::mov(X86::esp, X86::ebp);
    X86::pop(X86::ebp);
    X86::ret();
    X86::write("");

    currScope = fd.funBlock->outerBlock;
    return X86::Register();
  }
  X86::Register visit(If &i) override {
    /* Calculate Names for Blocks */
    const std::string trueBlockName =
        "ifTrueBlock" + std::to_string(blockCount++);
    const std::string falseBlockName =
        "ifFalseBlock" + std::to_string(blockCount++);
    const std::string endBlockName =
        "ifEndBlock" + std::to_string(blockCount++);

    /* Calculate the result of the if condition. */
    X86::Register condResReg = i.ifCondition->accept(*this);

    /* Branch to False block if False, else branch to True block. */
    X86::cmp(condResReg, 0);
    X86::jeq(falseBlockName);
    X86::jmp(trueBlockName);

    /* Handle when the Case is True. */
    X86::block(trueBlockName);
    i.ifBody->accept(*this);
    X86::jmp(endBlockName);

    /* Handle when the Case is False. */
    X86::block(falseBlockName);
    if (i.elseBody)
      i.elseBody->accept(*this);
    X86::jmp(endBlockName);

    /* Handle after the If statement. */
    X86::block(endBlockName);
    return X86::Register();
  }
  X86::Register visit(IntLiteral &il) override { return X86::Register(); }
  X86::Register visit(ParenthExpr &pe) override {
    pe.innerExpr->accept(*this);
    return X86::Register();
  }
  X86::Register visit(PointerType &pt) override { return X86::Register(); }
  X86::Register visit(Program &p) override {
    currScope = p.globalScope;

    X86::write(".DATA");
    for (std::shared_ptr<VarDecl> globalVar : p.globalVars)
      alloc(*globalVar);

    if (X86::macOS)
      X86::write(".globl _mainFunDecl");
    else
      X86::write(".globl mainFunDecl");

    for (std::shared_ptr<FunDecl> func : p.funDecls)
      func->accept(*this);

    return X86::Register();
  }
  X86::Register visit(Return &r) override {
    if (r.returnExpr)
      r.returnExpr->accept(*this);
    return X86::Register();
  }
  X86::Register visit(SizeOf &so) override { return X86::Register(); }
  X86::Register visit(StringLiteral &sl) override { return X86::Register(); }
  X86::Register visit(StructType &st) override { return X86::Register(); }
  X86::Register visit(StructTypeDecl &std) override { return X86::Register(); }
  X86::Register visit(TypeCast &tc) override { return tc.expr->accept(*this); }
  X86::Register visit(ValueAt &va) override {
    va.derefExpr->accept(*this);
    return X86::Register();
  }
  X86::Register visit(VarDecl &vd) override {
    int bytesRequired = vd.getBytes();
    currFpOffset -= bytesRequired;
    vd.fpOffset = currFpOffset;

    X86::sub(X86::esp, bytesRequired,
             "Allocated for VarDecl: " + vd.getIdentifier() +
                 " with fpOffset: " + std::to_string(currFpOffset));
    return X86::Register();
  }
  X86::Register visit(VarExpr &ve) override {
    /* Find this Variable's Location in the Stack, and Load It. */
    int fpOffset = ve.variableDecl->fpOffset;
    X86::comment("visit(VarExpr) with offset: " + std::to_string(fpOffset));
    if (fpOffset == 0)
      return X86::Register(0, ve.variableDecl->getIdentifier());

    if (fpOffset > 0)
      return X86::Register(0, "[ebp+" + std::to_string(fpOffset) + "]");
    else
      return X86::Register(0, "[ebp-" + std::to_string(fpOffset) + "]");
  }
  X86::Register visit(While &w) override {
    w.condition->accept(*this);
    w.body->accept(*this);
    return X86::Register();
  }
}; // namespace ACC
}; // namespace ACC

#endif