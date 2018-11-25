#pragma once

#ifndef DOTGRAPH_H
#define DOTGRAPH_H

#include <iostream>

#include "../AST.h"
#include "../ASTVisitor.h"

namespace ACC {

class DotGraph : public ASTVisitor {

public:
  DotGraph(const Program &progAST, std::ostream *out_stream)
      : progAST(progAST) {
    output = out_stream;
  }
  void print() { visit(progAST); }

private:
  int nodeCount = 0;
  std::ostream *output;
  const Program &progAST;

  void declare(const std::string &nodeID, const std::string &label) {
    put(nodeID + " [label=\"" + label + "\"];");
  }

  void join(const std::string &lhs, const std::string &rhs) {
    *output << lhs << " -> " << rhs << std::endl;
  }

  void put(const std::string &str) { *output << str << std::endl; }

  /* ---- Visit AST ---- */

  std::string visit(const ArrayAccess &aa) override {
    std::string arrayAccessID = "ArrayAccess" + std::to_string(nodeCount++);
    declare(arrayAccessID, "ArrayAccess");

    std::string arrayID = aa.array->accept(*this);
    std::string indexID = aa.index->accept(*this);

    join(arrayAccessID, arrayID);
    join(arrayAccessID, indexID);

    return arrayAccessID;
  }
  std::string visit(const ArrayType &at) override { return "undef"; }
  std::string visit(const Assign &as) override {
    std::string assignID = "Assign" + std::to_string(nodeCount++);
    declare(assignID, "=");

    std::string lhsID = as.lhs->accept(*this);
    std::string rhsID = as.rhs->accept(*this);

    join(assignID, lhsID);
    join(assignID, rhsID);

    return assignID;
  }
  std::string visit(const BaseType &bt) override { return "undef"; }
  std::string visit(const BinOp &bo) override {
    std::string binOpID = "BinOp" + std::to_string(nodeCount++);
    declare(binOpID, "BinOp");

    std::string lhsId = bo.lhs->accept(*this);

    std::string OpID = "Op" + std::to_string(nodeCount++);
    switch (bo.operation) {
    case Op::ADD:
      declare(OpID, "+");
      break;
    case Op::AND:
      declare(OpID, "&&");
      break;
    case Op::DIV:
      declare(OpID, "/");
      break;
    case Op::EQ:
      declare(OpID, "==");
      break;
    case Op::GE:
      declare(OpID, ">=");
      break;
    case Op::GT:
      declare(OpID, ">");
      break;
    case Op::LE:
      declare(OpID, "<=");
      break;
    case Op::LT:
      declare(OpID, "<");
      break;
    case Op::MOD:
      declare(OpID, "%");
      break;
    case Op::MUL:
      declare(OpID, "*");
      break;
    case Op::NE:
      declare(OpID, "!=");
      break;
    case Op::OR:
      declare(OpID, "||");
      break;
    case Op::SUB:
      declare(OpID, "-");
      break;
    }

    std::string rhsId = bo.rhs->accept(*this);

    join(binOpID, lhsId);
    join(binOpID, OpID);
    join(binOpID, rhsId);

    return binOpID;
  }
  std::string visit(const Block &b) override {
    std::string blockID = "Block" + std::to_string(nodeCount++);
    declare(blockID, "{}");
    for (const auto stmt : b.blockStmts)
      join(blockID, stmt->accept(*this));
    return blockID;
  }
  std::string visit(const CharLiteral &cl) override {
    std::string charID = "CharLiteral" + std::to_string(nodeCount++);
    declare(charID, "'" + cl.value + "'");
    return charID;
  }
  std::string visit(const FieldAccess &fa) override {
    std::string fieldAccessID = "FieldAccess" + std::to_string(nodeCount++);
    declare(fieldAccessID, "FieldAccess." + fa.field);

    std::string objID = fa.object->accept(*this);

    join(fieldAccessID, objID);

    return fieldAccessID;
  }
  std::string visit(const FunCall &fc) override {
    std::string funCallID = "FunCall" + std::to_string(nodeCount++);
    declare(funCallID, fc.funName + "()");

    for (const auto arg : fc.funArgs)
      join(funCallID, arg->accept(*this));

    return funCallID;
  }
  std::string visit(const FunDecl &fd) override {
    std::string funcID = "FunDecl" + std::to_string(nodeCount++);
    declare(funcID, fd.funName);
    join(funcID, fd.funBlock->accept(*this));
    return funcID;
  }
  std::string visit(const If &i) override {
    std::string ifID = "If" + std::to_string(nodeCount++);
    declare(ifID, "if () {} else {}");

    join(ifID, i.ifCondition->accept(*this));
    join(ifID, i.ifBody->accept(*this));
    if (i.elseBody != nullptr) {
      join(ifID, i.elseBody->accept(*this));
    }

    return ifID;
  }
  std::string visit(const IntLiteral &il) override {
    std::string intID = "IntLiteral" + std::to_string(nodeCount++);
    declare(intID, il.value);
    return intID;
  }
  std::string visit(const ParenthExpr &pe) override {
    return pe.innerExpr->accept(*this);
  }
  std::string visit(const PointerType &pt) override { return "undef"; }
  std::string visit(const Program &p) override {
    std::cout << "digraph prog {" << std::endl;
    for (const auto decl : p.decls) {
      decl->accept(*this);
    }
    std::cout << "}" << std::endl;
    return "Node0";
  }
  std::string visit(const Return &r) override {
    std::string returnID = "Return" + std::to_string(nodeCount++);
    declare(returnID, "return");

    if (r.returnExpr != nullptr) {
      join(returnID, r.returnExpr->accept(*this));
    }

    return returnID;
  }
  std::string visit(const SizeOf &so) override {
    std::string returnID = "SizeOf" + std::to_string(nodeCount++);
    declare(returnID, "sizeof(" + so.type->accept(*this) + ")");

    return returnID;
  }
  std::string visit(const StringLiteral &sl) override {
    std::string strID = "StringLiteral" + std::to_string(nodeCount++);
    declare(strID, "\"" + sl.value + "\"");
    return strID;
  }
  std::string visit(const StructType &st) override { return "undef"; }
  std::string visit(const StructTypeDecl &std) override { return "undef"; }
  std::string visit(const TypeCast &tc) override { return "undef"; }
  std::string visit(const ValueAt &va) override { return "undef"; }
  std::string visit(const VarDecl &vd) override {
    nodeCount++;
    std::string varDeclID = "VarDecl" + std::to_string(nodeCount++);
    declare(varDeclID, "let " + vd.identifer + ";");
    return varDeclID;
  }
  std::string visit(const VarExpr &ve) override {
    std::string varID = "VarExpr" + std::to_string(nodeCount++);
    declare(varID, ve.identifier);
    return varID;
  }
  std::string visit(const While &w) override { return "undef"; }
};

}; // namespace ACC

#endif