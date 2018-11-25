#pragma once

#ifndef NAMEANALYSIS_H
#define NAMEANALYSIS_H

#include <iostream>
#include <memory>
#include <set>

#include "../AST.h"
#include "../ASTVisitor.h"

namespace ACC {

class NameAnalysis : public ASTVisitor<void> {

public:
  int errorCount = 0;

  NameAnalysis(Program &progAST, std::ostream *out_stream = &std::cout)
      : progAST(progAST) {
    output = out_stream;
  }
  void error(std::string error) {
    errorCount++;
    *output << error << std::endl;
  }
  void run() { visit(progAST); }

private:
  int nodeCount = 0;
  std::ostream *output;
  Program &progAST;

  std::shared_ptr<Block> currScope;

  /* ---- Visit AST ---- */

  void visit(ArrayAccess &aa) override { aa.array->accept(*this); }
  void visit(ArrayType &at) override {}
  void visit(Assign &as) override {
    as.lhs->accept(*this);
    as.rhs->accept(*this);
  }
  void visit(BaseType &bt) override {}
  void visit(BinOp &bo) override {
    bo.lhs->accept(*this);
    bo.rhs->accept(*this);
  }
  void visit(Block &b) override {
    if (b.outerBlock == nullptr) {
      b.setOuterBlock(currScope);
      currScope = std::make_shared<Block>(b);
    }
    for (const auto &stmt : b.blockStmts)
      stmt->accept(*this);
    currScope = b.outerBlock;
  }
  void visit(CharLiteral &cl) override {}
  void visit(FieldAccess &fa) override {
    fa.object->accept(*this);
    // TODO Assert Struct has said Field.
  }
  void visit(FunCall &fc) override {
    if (currScope->find(fc.funName) == nullptr)
      return error("Attempted to call undeclared function: " + fc.funName);
    for (const auto &arg : fc.funArgs)
      arg->accept(*this);
  }
  void visit(FunDecl &fd) override {
    if (currScope->findLocal(fd.getIdentifier()))
      return error("Attempted to declare a Function with an identifier that is "
                   "already in use: " +
                   fd.getIdentifier());
    currScope->insertDecl(std::make_shared<FunDecl>(fd));

    fd.funBlock->setOuterBlock(currScope);
    currScope = fd.funBlock;

    for (const auto &param : fd.funParams)
      param->accept(*this);
    fd.funBlock->accept(*this);
    currScope = fd.funBlock->outerBlock;
  }
  void visit(If &i) override {
    i.ifCondition->accept(*this);
    i.ifBody->accept(*this);
    if (i.elseBody)
      i.elseBody->accept(*this);
  }
  void visit(IntLiteral &il) override {}
  void visit(ParenthExpr &pe) override { pe.innerExpr->accept(*this); }
  void visit(PointerType &pt) override {}
  void visit(Program &p) override {
    currScope = std::make_shared<Block>(Block({}));
    for (const std::shared_ptr<Decl> &decl : p.decls) {
      decl->accept(*this);
    }
    p.setGlobalScope(currScope);
  }
  void visit(Return &r) override {
    if (r.returnExpr)
      r.returnExpr->accept(*this);
  }
  void visit(SizeOf &so) override {}
  void visit(StringLiteral &sl) override {}
  void visit(StructType &st) override {}
  void visit(StructTypeDecl &std) override {
    if (currScope->findLocal(std.getIdentifier()))
      return error("Attempted to declare a Struct with an identifier that is "
                   "already in use: " +
                   std.getIdentifier());

    currScope->insertDecl(std::make_shared<StructTypeDecl>(std));

    /* Check that the fields in this struct are unique */
    std::set<std::string> structTypeFields;
    for (const std::shared_ptr<VarDecl> field : std.varDecls) {
      if (structTypeFields.find(field->getIdentifier()) !=
          structTypeFields.end())
        return error("Struct " + std.getIdentifier() +
                     " contained multiple fields with the same identifier: " +
                     field->getIdentifier());
      structTypeFields.insert(field->identifer);
    }
  }
  void visit(TypeCast &tc) override { tc.expr->accept(*this); }
  void visit(ValueAt &va) override { va.derefExpr->accept(*this); }
  void visit(VarDecl &vd) override {
    if (currScope->findLocal(vd.getIdentifier()))
      return error("Attempted to declare a Variable with an identifier that is "
                   "already in use: " +
                   vd.getIdentifier());
    currScope->insertDecl(std::make_shared<VarDecl>(vd));
  }
  void visit(VarExpr &ve) override {
    if (currScope->find(ve.identifier) == nullptr)
      return error("Attempted to reference undeclared variable: " +
                   ve.identifier);
  }
  void visit(While &w) override {
    w.condition->accept(*this);
    w.body->accept(*this);
  }
};

}; // namespace ACC

#endif