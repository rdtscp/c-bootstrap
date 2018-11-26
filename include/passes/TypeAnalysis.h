#pragma once

#ifndef TYPEANALYSIS_H
#define TYPEANALYSIS_H

#include <iostream>
#include <memory>
#include <set>
#include <vector>

#include "../AST.h"
#include "../ASTVisitor.h"

namespace ACC {

class TypeAnalysis : public ASTVisitor<std::shared_ptr<Type>> {

public:
  int errorCount = 0;
  std::vector<std::string> errors;

  TypeAnalysis(Program &progAST) : progAST(progAST) {}

  std::shared_ptr<Type> error(std::string error) {
    errorCount++;
    errors.push_back(error);
    return nullptr;
  }

  void printErrors() {
    std::cout << "Type Analysis Errors:" << std::endl;
    for (const auto &error : errors)
      std::cout << "\t" << error << std::endl;
  }

  void run() { visit(progAST); }

private:
  int nodeCount = 0;
  std::ostream *output;
  Program &progAST;

  std::shared_ptr<Block> currScope;

  /* ---- Visit AST ---- */

  std::shared_ptr<Type> visit(ArrayAccess &aa) override {
    std::shared_ptr<Type> arrayExprType = aa.array->accept(*this);
    std::shared_ptr<Type> arrayIndex = aa.index->accept(*this);
    if (arrayExprType->astClass() != "ArrayType")
      return error(
          "Type Error: Attempted to index an expression which was not an "
          "array. Was of type: " +
          arrayExprType->astClass());
    if (arrayIndex->astClass() != "BaseType")
      return error(
          "Type Error: Attempted to index an array using an expression which "
          "was not of type int. Was of type: " +
          arrayIndex->astClass());

    return std::make_shared<ArrayType>(
               *static_cast<ArrayType *>(arrayExprType.get()))
        ->arrayType;
  }
  std::shared_ptr<Type> visit(ArrayType &at) override {
    return std::make_shared<ArrayType>(at);
  }
  std::shared_ptr<Type> visit(Assign &as) override {
    std::shared_ptr<Type> lhs = as.lhs->accept(*this);
    std::shared_ptr<Type> rhs = as.rhs->accept(*this);
    if (*lhs != *rhs)
      return error("Assignation has mismatched types.");
    return nullptr;
  }
  std::shared_ptr<Type> visit(BaseType &bt) override {
    return std::make_shared<BaseType>(bt);
  }
  std::shared_ptr<Type> visit(BinOp &bo) override {
    bo.lhs->accept(*this);
    bo.rhs->accept(*this);
    return std::make_shared<BaseType>(BaseType(PrimitiveType::INT));
  }
  std::shared_ptr<Type> visit(Block &b) override {
    if (b.outerBlock == nullptr) {
      b.setOuterBlock(currScope);
      currScope = std::make_shared<Block>(b);
    }
    for (const auto &stmt : b.blockStmts)
      stmt->accept(*this);
    currScope = b.outerBlock;
    return nullptr;
  }
  std::shared_ptr<Type> visit(CharLiteral &cl) override {
    return std::make_shared<BaseType>(BaseType(PrimitiveType::CHAR));
  }
  std::shared_ptr<Type> visit(FieldAccess &fa) override {
    std::shared_ptr<Type> objType = fa.object->accept(*this);
    if (objType->astClass() != "StructType")
      return error("Type Analysis: Attempted to access field on expression "
                   "that is not a struct");

    std::shared_ptr<StructType> structType =
        std::make_shared<StructType>(*static_cast<StructType *>(objType.get()));

    std::shared_ptr<Decl> identDecl = currScope->find(structType->identifier);
    if (identDecl == nullptr)
      return error("Type Analysis: Attempted to access field on expression "
                   "that does not have a type definition.");
    if (identDecl->astClass() != "StructTypeDecl")
      return error("Type Analysis: Attempted to access field on expression "
                   "that does not have a StructTypeDecl");

    std::shared_ptr<StructTypeDecl> structTypeDecl =
        std::make_shared<StructTypeDecl>(
            *static_cast<StructTypeDecl *>(identDecl.get()));

    for (const auto &field : structTypeDecl->varDecls)
      if (field->identifer == fa.field)
        return field->type;

    return error("Type Analysis: Attempted to access field on a struct that "
                 "does not exist.");
  }
  std::shared_ptr<Type> visit(FunCall &fc) override {
    std::shared_ptr<Decl> identDecl = currScope->find(fc.funName);
    if (identDecl == nullptr)
      return error("Type Analysis: Attempted to call undeclared function: " +
                   fc.funName);

    if (identDecl->astClass() != "FunDecl")
      return error("Type Analysis: Attempted to call undeclared function: " +
                   fc.funName);

    std::shared_ptr<FunDecl> funDecl =
        std::make_shared<FunDecl>(*static_cast<FunDecl *>(identDecl.get()));

    if (funDecl->funParams.size() != fc.funArgs.size())
      return error("Type Analysis: Attempted to call function: " + fc.funName +
                   " with incorrect number of arguments");

    for (int i = 0; i < fc.funArgs.size(); i++) {
      std::shared_ptr<Type> argType = fc.funArgs[i]->accept(*this);
      std::shared_ptr<Type> paramType = funDecl->funParams[i]->type;
      if (*argType != *paramType)
        return error("Type Analysis: Attempted to call function: " +
                     fc.funName + " with arguments of incorrect type.");
    }
    return funDecl->funType;
  }
  std::shared_ptr<Type> visit(FunDecl &fd) override {
    fd.funBlock->setOuterBlock(currScope);
    currScope = fd.funBlock;

    for (const auto &param : fd.funParams)
      param->accept(*this);
    fd.funBlock->accept(*this);
    currScope = fd.funBlock->outerBlock;
    return nullptr;
  }
  std::shared_ptr<Type> visit(If &i) override {
    std::shared_ptr<Type> condType = i.ifCondition->accept(*this);

    if (condType->astClass() != "BaseType")
      return error("Type Analysis: Type of While condition is invalid.");

    std::shared_ptr<BaseType> condBaseType =
        std::make_shared<BaseType>(*static_cast<BaseType *>(condType.get()));

    if (condBaseType->primitiveType != PrimitiveType::INT)
      return error("Type Analysis: Type of While condition is not INT.");

    i.ifBody->accept(*this);

    if (i.elseBody)
      i.elseBody->accept(*this);
    return nullptr;
  }
  std::shared_ptr<Type> visit(IntLiteral &il) override {
    return std::make_shared<BaseType>(BaseType(PrimitiveType::INT));
  }
  std::shared_ptr<Type> visit(ParenthExpr &pe) override {
    return pe.innerExpr->accept(*this);
  }
  std::shared_ptr<Type> visit(PointerType &pt) override {
    return std::make_shared<PointerType>(pt);
  }
  std::shared_ptr<Type> visit(Program &p) override {
    currScope = p.getGlobalScope();
    for (const std::shared_ptr<Decl> &decl : p.decls)
      decl->accept(*this);
    return nullptr;
  }
  std::shared_ptr<Type> visit(Return &r) override {
    if (r.returnExpr)
      return r.returnExpr->accept(*this);
    return std::make_shared<BaseType>(BaseType(PrimitiveType::VOID));
    ;
  }
  std::shared_ptr<Type> visit(SizeOf &so) override {
    return std::make_shared<BaseType>(BaseType(PrimitiveType::INT));
  }
  std::shared_ptr<Type> visit(StringLiteral &sl) override { return nullptr; }
  std::shared_ptr<Type> visit(StructType &st) override {
    return std::make_shared<StructType>(st);
  }
  std::shared_ptr<Type> visit(StructTypeDecl &std) override { return nullptr; }
  std::shared_ptr<Type> visit(TypeCast &tc) override { return tc.type; }
  std::shared_ptr<Type> visit(ValueAt &va) override {
    std::shared_ptr<Type> exprType = va.derefExpr->accept(*this);
    if (exprType->astClass() != "PointerType")
      return error("Attempted to dereference variable that wasn't a pointer. "
                   "Was of type: " +
                   exprType->astClass());
    return std::make_shared<PointerType>(
               *static_cast<PointerType *>(exprType.get()))
        ->pointedType;
  }
  std::shared_ptr<Type> visit(VarDecl &vd) override { return nullptr; }
  std::shared_ptr<Type> visit(VarExpr &ve) override {
    std::shared_ptr<Decl> identDecl = currScope->find(ve.identifier);

    if (identDecl->astClass() != "VarDecl")
      return error("Attempted to reference " + identDecl->astClass() +
                   " as a variable.");
    return std::make_shared<VarDecl>(*static_cast<VarDecl *>(identDecl.get()))
        ->type;
  }
  std::shared_ptr<Type> visit(While &w) override {
    std::shared_ptr<Type> conditionType = w.condition->accept(*this);
    if (conditionType->astClass() != "BaseType")
      return error("Type Analysis: Type of While condition is invalid.");
    std::shared_ptr<BaseType> conditionBaseType = std::make_shared<BaseType>(
        *static_cast<BaseType *>(conditionType.get()));
    if (conditionBaseType->primitiveType != PrimitiveType::INT)
      return error("Type Analysis: Type of While condition is not INT.");
    w.body->accept(*this);
    return nullptr;
  }
};

}; // namespace ACC

#endif