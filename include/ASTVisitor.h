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
  virtual void visit(const ArrayAccess &aa) = 0;
  virtual void visit(const ArrayType &at) = 0;
  virtual void visit(const Assign &as) = 0;
  virtual void visit(const BaseType &bt) = 0;
  virtual void visit(const BinOp &bo) = 0;
  virtual void visit(const Block &b) = 0;
  virtual void visit(const CharLiteral &cl) = 0;
  virtual void visit(const FieldAccess &fa) = 0;
  virtual void visit(const FunCall &fc) = 0;
  virtual void visit(const FunDecl &fd) = 0;
  virtual void visit(const If &i) = 0;
  virtual void visit(const IntLiteral &il) = 0;
  virtual void visit(const ParenthExpr &pe) = 0;
  virtual void visit(const PointerType &pt) = 0;
  virtual void visit(const Program &p) = 0;
  virtual void visit(const Return &r) = 0;
  virtual void visit(const SizeOf &so) = 0;
  virtual void visit(const StringLiteral &sl) = 0;
  virtual void visit(const StructType &st) = 0;
  virtual void visit(const StructTypeDecl &std) = 0;
  virtual void visit(const TypeCast &tc) = 0;
  virtual void visit(const ValueAt &va) = 0;
  virtual void visit(const VarDecl &vd) = 0;
  virtual void visit(const VarExpr &ve) = 0;
  virtual void visit(const While &w) = 0;
};

}; // namespace ACC

#endif