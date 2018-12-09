#pragma once

#ifndef GENERATEMIPS_H
#define GENERATEMIPS_H

#include <algorithm>
#include <fstream>
#include <iostream>
#include <memory>
#include <set>
#include <stack>
#include <vector>

#include "../AST.h"
#include "../ASTVisitor.h"
#include "Registers.h"

namespace ACC {

class GenerateMIPS : public ASTVisitor<MIPS::Register> {

public:
  int errorCount = 0;
  std::vector<std::string> errors;

  GenerateMIPS(Program &progAST, std::string outputFile) : progAST(progAST) {
    out.open(outputFile);
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
    out.close();
  }

private:
  int nodeCount = 0;
  std::ofstream out;
  Program &progAST;

  std::shared_ptr<Block> currScope;
  std::stack<MIPS::Register> freeRegs;

  /* ---- MIPS Operations ---- */

  void comment(const std::string &comment) {
    out << "#" << comment << std::endl;
  }

  void ADDI(const MIPS::Register &dest, const MIPS::Register &src,
            const int val) {
    out << "ADDI " << dest.toString() << ", " << src.toString() << ", " << val;
    out << "\t# " << dest.toString() << " = " << src.toString() << " + " << val;
    out << std::endl;
  }

  void JAL(const std::string &blockName) {
    out << "JAL " << blockName << std::endl;
  }

  void LW(const MIPS::Register &destReg, const int addr, const int offset = 0) {
    out << "LW " << destReg.toString() << ", " << offset << "(" << addr << ")";
    out << "\t# " << destReg.toString() << " = " << offset << "(" << addr
        << ")";
    out << std::endl;
  }

  void LW(const MIPS::Register &destReg, const MIPS::Register &addr,
          const int offset = 0) {
    out << "LW " << destReg.toString() << ", " << offset << "("
        << addr.toString() << ")";
    out << "\t# " << destReg.toString() << " = " << offset << "(" << addr
        << ")";
    out << std::endl;
  }

  void SW(const MIPS::Register &regCtnt, const int addr, const int offset = 0) {
    out << "SW " << regCtnt.toString() << ", " << offset << "(" << addr << ")";
    out << "\t# " << offset << "(" << addr << ") = " << regCtnt.toString();
    out << std::endl;
  }

  void SW(const MIPS::Register &regCtnt, const MIPS::Register &addr,
          const int offset = 0) {
    out << "SW " << regCtnt.toString() << ", " << offset << "("
        << addr.toString() << ")";
    out << "\t# " << offset << "(" << addr.toString()
        << ") = " << regCtnt.toString();
    out << std::endl;
  }
  /* ---- Register Management ---- */

  void freeAllRegs() {
    for (auto rit = MIPS::tmpRegs.rbegin(); rit < MIPS::tmpRegs.rend(); rit++)
      freeRegs.push(*rit);
  }

  void freeRegister(MIPS::Register reg) {
    if (reg == MIPS::v0)
      return;
    freeRegs.push(reg);
  }

  MIPS::Register getRegister() {
    if (freeRegs.size() > 0) {
      MIPS::Register reg = freeRegs.top();
      freeRegs.pop();
      return reg;
    }
    throw std::runtime_error(
        "MIPS Generation: Attempted to re-use an already allocated Register.ƒ");
  }

  void writeRegsState() {
    comment(" ~~~ Writing Reg State to Stack ~~~ ");
    for (const MIPS::Register &reg : MIPS::tmpRegs) {
      ADDI(MIPS::sp, MIPS::sp, -4);
      SW(reg, MIPS::sp);
    }
    comment(" ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ ");
  }

  void readRegsState() {
    std::vector<MIPS::Register> tmpRegsReverse = MIPS::tmpRegs;
    std::reverse(std::begin(tmpRegsReverse), std::end(tmpRegsReverse));

    comment(" ~~~ Reading Reg State from Stack ~~~ ");
    for (const MIPS::Register &reg : MIPS::tmpRegs) {
      LW(reg, MIPS::sp);
      ADDI(MIPS::sp, MIPS::sp, 4);
    }
    comment(" ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ ");
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
    return MIPS::Register();
  }
  MIPS::Register visit(FunDecl &fd) override {
    currScope = fd.funBlock;

    for (const auto &param : fd.funParams)
      param->accept(*this);
    fd.funBlock->accept(*this);

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
    out << "# ---- Generating MIPS for Program ---- " << std::endl;
    currScope = p.globalScope;

    std::stack<MIPS::Register> reinstateFreeRegs = freeRegs;
    writeRegsState();
    freeAllRegs();

    JAL("mainFuncDecl");

    for (const std::shared_ptr<Decl> &decl : p.decls)
      decl->accept(*this);

    readRegsState();
    freeRegs = reinstateFreeRegs;
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
  MIPS::Register visit(VarDecl &vd) override { return MIPS::Register(); }
  MIPS::Register visit(VarExpr &ve) override { return MIPS::Register(); }
  MIPS::Register visit(While &w) override {
    w.condition->accept(*this);
    w.body->accept(*this);
    return MIPS::Register();
  }
};

}; // namespace ACC

#endif