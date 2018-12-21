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

  GenerateMIPS(Program &progAST, std::string outputFile) : progAST(progAST) {
    MIPS::mipsOutput.open(outputFile);
  }

  void error(std::string error) {
    errorCount++;
    errors.push_back(error);
  }

  void printErrors() const {
    std::cerr << "Name Analysis Errors:" << std::endl;
    for (const auto &error : errors)
      std::cerr << "\t" << error << std::endl;
  }

  void run() {
    freeAllRegs();
    visit(progAST);
    MIPS::mipsOutput.close();
  }

private:
  Program &progAST;

  std::shared_ptr<Block> currScope;
  int currFpOffset = 0;

  /* ---- Register Management ---- */

  std::stack<MIPS::Register> freeArgsRegs;
  std::stack<MIPS::Register> freeSaveRegs;
  std::stack<MIPS::Register> freeTempRegs;

  void freeAllRegs() {
    freeAllArgsRegs();
    freeAllSaveRegs();
    freeAllTempRegs();
  }

  void freeAllArgsRegs() {
    for (auto rit = MIPS::argsRegs.rbegin(); rit < MIPS::argsRegs.rend(); rit++)
      freeArgsRegs.push(*rit);
  }

  void freeAllSaveRegs() {
    for (auto rit = MIPS::saveRegs.rbegin(); rit < MIPS::saveRegs.rend(); rit++)
      freeSaveRegs.push(*rit);
  }

  void freeAllTempRegs() {
    for (auto rit = MIPS::tempRegs.rbegin(); rit < MIPS::tempRegs.rend(); rit++)
      freeTempRegs.push(*rit);
  }

  void freeRegister(MIPS::Register reg) {
    switch (reg.name[0]) {
    case 'a':
      freeArgsRegs.push(reg);
      break;
    case 's':
      freeSaveRegs.push(reg);
      break;
    case 't':
      freeTempRegs.push(reg);
      break;
    default:
      break;
    }
  }

  MIPS::Register getArgsRegister() {
    if (freeArgsRegs.size() > 0) {
      MIPS::Register reg = freeArgsRegs.top();
      freeArgsRegs.pop();
      return reg;
    }
    throw std::runtime_error("MIPS Generation: Attempted to re-use an already "
                             "allocated Args Register.");
  }

  MIPS::Register getSaveRegister() {
    if (freeSaveRegs.size() > 0) {
      MIPS::Register reg = freeSaveRegs.top();
      freeSaveRegs.pop();
      return reg;
    }
    throw std::runtime_error("MIPS Generation: Attempted to re-use an already "
                             "allocated Save Register.");
  }

  MIPS::Register getTempRegister() {
    if (freeTempRegs.size() > 0) {
      MIPS::Register reg = freeTempRegs.top();
      freeTempRegs.pop();
      return reg;
    }
    throw std::runtime_error("MIPS Generation: Attempted to re-use an already "
                             "allocated Temp Register.");
  }

  /* ---- MIPS Memory ---- */

  void alloc(const int bytes) {
    MIPS::comment(" !--- Cannot Allocate on Heap yet ----! ");
  }

  void stackPush(const MIPS::Register &src) {
    MIPS::ADDI(MIPS::sp, MIPS::sp, -4);
    MIPS::SW(src, MIPS::sp);
  }

  void stackPop(const MIPS::Register &dest) {
    MIPS::LW(dest, MIPS::sp);
    MIPS::ADDI(MIPS::sp, MIPS::sp, 4);
  }

  /* ---- Visit AST ---- */

  MIPS::Register visit(ArrayAccess &aa) override {
    aa.array->accept(*this);
    return MIPS::Register();
  }
  MIPS::Register visit(ArrayType &at) override { return MIPS::Register(); }
  MIPS::Register visit(Assign &as) override {
    as.lhs->accept(*this);
    as.rhs->accept(*this);
    return MIPS::Register();
  }
  MIPS::Register visit(BaseType &bt) override { return MIPS::Register(); }
  MIPS::Register visit(BinOp &bo) override {
    bo.lhs->accept(*this);
    bo.rhs->accept(*this);
    return MIPS::Register();
  }
  MIPS::Register visit(Block &b) override {
    currScope = std::make_shared<Block>(b);
    for (const auto &stmt : b.blockStmts)
      stmt->accept(*this);

    /* Clean up the stack. */
    for (const auto &ident_decl : b.blockDecls) {
      std::shared_ptr<Decl> currDecl = ident_decl.second;
      if (currDecl->astClass() == "VarDecl") {
        /* ---- Deconstruct VarDecl ---- */
        MIPS::ADDI(MIPS::sp, MIPS::sp, 4);
        MIPS::SW(MIPS::zero, MIPS::sp);
      }
    }
    currScope = b.outerBlock;
    return MIPS::Register();
  }
  MIPS::Register visit(CharLiteral &cl) override { return MIPS::Register(); }
  MIPS::Register visit(FieldAccess &fa) override {
    fa.object->accept(*this);
    return MIPS::Register();
  }
  MIPS::Register visit(FunCall &fc) override {

    for (const auto &arg : fc.funArgs)
      arg->accept(*this);
    MIPS::JAL(fc.funName  + "FunDecl");
    return MIPS::Register();
  }
  MIPS::Register visit(FunDecl &fd) override {
    std::vector<MIPS::Register> saveRegs = MIPS::saveRegs;
    currScope = fd.funBlock;

    MIPS::BLOCK(fd.getIdentifier() + "FunDecl");

    /* ---- Save Caller's $fp ---- */
    stackPush(MIPS::fp);

    /* ---- Save Registers [ $s0-$s7 ] && $ra ---- */
    for (const MIPS::Register &saveReg : saveRegs)
      stackPush(saveReg);
    stackPush(MIPS::ra);

    /* ---- Construct Arguments ---- */
    // @TODO

    /* ---- Execute Function ---- */

    fd.funBlock->accept(*this);

    /* -------------------------- */

    /* ---- Deconstruct Arguments ---- */
    // @TODO

    /* ---- Load Registers [ $s0-$s7 ] && $ra ---- */
    stackPop(MIPS::ra);
    std::reverse(std::begin(saveRegs), std::end(saveRegs));
    for (const MIPS::Register &saveReg : saveRegs)
      stackPop(saveReg);

    /* ---- Load Caller's $fp ---- */
    stackPop(MIPS::fp);

    currScope = fd.funBlock->outerBlock;
    return MIPS::Register();
  }
  MIPS::Register visit(If &i) override {
    i.ifCondition->accept(*this);
    i.ifBody->accept(*this);
    if (i.elseBody)
      i.elseBody->accept(*this);
    return MIPS::Register();
  }
  MIPS::Register visit(IntLiteral &il) override { return MIPS::Register(); }
  MIPS::Register visit(ParenthExpr &pe) override {
    pe.innerExpr->accept(*this);
    return MIPS::Register();
  }
  MIPS::Register visit(PointerType &pt) override { return MIPS::Register(); }
  MIPS::Register visit(Program &p) override {
    currScope = p.globalScope;

    freeAllRegs();

    MIPS::write(".data");
    for (std::shared_ptr<VarDecl> globalVar : p.globalVars)
      MIPS::alloc(globalVar->getIdentifier(), globalVar->getBytes());

    MIPS::write(".text");
    MIPS::JAL("mainFunDecl");
    for (std::shared_ptr<FunDecl> func : p.funDecls)
      func->accept(*this);

    return MIPS::Register();
  }
  MIPS::Register visit(Return &r) override {
    if (r.returnExpr)
      r.returnExpr->accept(*this);
    return MIPS::Register();
  }
  MIPS::Register visit(SizeOf &so) override { return MIPS::Register(); }
  MIPS::Register visit(StringLiteral &sl) override { return MIPS::Register(); }
  MIPS::Register visit(StructType &st) override { return MIPS::Register(); }
  MIPS::Register visit(StructTypeDecl &std) override {
    return MIPS::Register();
  }
  MIPS::Register visit(TypeCast &tc) override { return tc.expr->accept(*this); }
  MIPS::Register visit(ValueAt &va) override {
    va.derefExpr->accept(*this);
    return MIPS::Register();
  }
  MIPS::Register visit(VarDecl &vd) override {
    MIPS::comment("Allocating for VarDecl: " + vd.getIdentifier());
    int bytesRequired = vd.getBytes();
    MIPS::ADDI(MIPS::sp, MIPS::sp, -bytesRequired);
    currFpOffset -= bytesRequired;
    vd.fpOffset = currFpOffset;
    return MIPS::Register();
  }
  MIPS::Register visit(VarExpr &ve) override {
    /* Find this Variable's Location in the Stack, and Load It. */
    int fpOffset = ve.variableDecl->fpOffset;
    MIPS::Register valReg = getTempRegister();
    MIPS::LW(valReg, MIPS::fp, fpOffset);
    return valReg;
  }
  MIPS::Register visit(While &w) override {
    w.condition->accept(*this);
    w.body->accept(*this);
    return MIPS::Register();
  }
}; // namespace ACC
}; // namespace ACC

#endif