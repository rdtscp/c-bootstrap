#pragma once

#ifndef ASTVISITOR_H
#define ASTVISITOR_H

#include "AST.h"

namespace ACC {

class ArrayAccess;
class ArrayType;
class Assign;
class BaseType;
class BinOp;
class Block;
class CharLiteral;
class FieldAccess;
class FunCall;
class FunDecl;
class If;
class IntLiteral;
class ParenthExpr;
class PointerType;
class Program;
class Return;
class SizeOf;
class StringLiteral;
class StructType;
class StructTypeDecl;
class TypeCast;
class ValueAt;
class VarDecl;
class VarExpr;
class While;

class ASTVisitor {

public:
  virtual std::string visit(ArrayAccess &aa) = 0;
  virtual std::string visit(ArrayType &at) = 0;
  virtual std::string visit(Assign &as) = 0;
  virtual std::string visit(BaseType &bt) = 0;
  virtual std::string visit(BinOp &bo) = 0;
  virtual std::string visit(Block &b) = 0;
  virtual std::string visit(CharLiteral &cl) = 0;
  virtual std::string visit(FieldAccess &fa) = 0;
  virtual std::string visit(FunCall &fc) = 0;
  virtual std::string visit(FunDecl &fd) = 0;
  virtual std::string visit(If &i) = 0;
  virtual std::string visit(IntLiteral &il) = 0;
  virtual std::string visit(ParenthExpr &pe) = 0;
  virtual std::string visit(PointerType &pt) = 0;
  virtual std::string visit(Program &p) = 0;
  virtual std::string visit(Return &r) = 0;
  virtual std::string visit(SizeOf &so) = 0;
  virtual std::string visit(StringLiteral &sl) = 0;
  virtual std::string visit(StructType &st) = 0;
  virtual std::string visit(StructTypeDecl &std) = 0;
  virtual std::string visit(TypeCast &tc) = 0;
  virtual std::string visit(ValueAt &va) = 0;
  virtual std::string visit(VarDecl &vd) = 0;
  virtual std::string visit(VarExpr &ve) = 0;
  virtual std::string visit(While &w) = 0;
};

}; // namespace ACC

#endif