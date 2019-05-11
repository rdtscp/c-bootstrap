#pragma once

namespace ACC {

class AddressOf;
class Allocation;
class ArrayType;
class Assign;
class BaseType;
class BinOp;
class Block;
class BoolLiteral;
class CharLiteral;
class ClassType;
class ClassTypeDecl;
class ClassTypeDef;
class ConstructorDecl;
class ConstructorDef;
class Deletion;
class DestructorDecl;
class DestructorDef;
class DoWhile;
class EnumClassTypeDecl;
class For;
class FunCall;
class FunDecl;
class FunDef;
class Identifier;
class If;
class IntLiteral;
class MemberAccess;
class MemberCall;
class Namespace;
class Nullptr;
class ParenthExpr;
class PointerType;
class PrefixOp;
class Program;
class ReferenceType;
class Return;
class SizeOf;
class StringLiteral;
class SubscriptOp;
class TertiaryExpr;
class Throw;
class TypeCast;
class TypeDefDecl;
class ValueAt;
class VarDecl;
class VarDef;
class VarExpr;
class While;

template <typename T> class ASTVisitor {

public:
  virtual ~ASTVisitor() {}

  virtual T visit(AddressOf &ao) = 0;
  virtual T visit(Allocation &a) = 0;
  virtual T visit(ArrayType &at) = 0;
  virtual T visit(Assign &as) = 0;
  virtual T visit(BaseType &bt) = 0;
  virtual T visit(BinOp &bo) = 0;
  virtual T visit(Block &b) = 0;
  virtual T visit(BoolLiteral &bl) = 0;
  virtual T visit(CharLiteral &cl) = 0;
  virtual T visit(ClassType &ct) = 0;
  virtual T visit(ClassTypeDecl &ctd) = 0;
  virtual T visit(ClassTypeDef &ctd) = 0;
  virtual T visit(ConstructorDecl &cd) = 0;
  virtual T visit(ConstructorDef &cd) = 0;
  virtual T visit(Deletion &d) = 0;
  virtual T visit(DestructorDecl &dd) = 0;
  virtual T visit(DestructorDef &dd) = 0;
  virtual T visit(DoWhile &dw) = 0;
  virtual T visit(EnumClassTypeDecl &ectd) = 0;
  virtual T visit(For &f) = 0;
  virtual T visit(FunCall &fc) = 0;
  virtual T visit(FunDecl &fd) = 0;
  virtual T visit(FunDef &fd) = 0;
  virtual T visit(Identifier &i) = 0;
  virtual T visit(If &i) = 0;
  virtual T visit(IntLiteral &il) = 0;
  virtual T visit(MemberAccess &ma) = 0;
  virtual T visit(MemberCall &mc) = 0;
  virtual T visit(Namespace &n) = 0;
  virtual T visit(Nullptr &n) = 0;
  virtual T visit(ParenthExpr &pe) = 0;
  virtual T visit(PointerType &pt) = 0;
  virtual T visit(PrefixOp &pi) = 0;
  virtual T visit(Program &p) = 0;
  virtual T visit(ReferenceType &rt) = 0;
  virtual T visit(Return &r) = 0;
  virtual T visit(SizeOf &so) = 0;
  virtual T visit(StringLiteral &sl) = 0;
  virtual T visit(SubscriptOp &so) = 0;
  virtual T visit(TertiaryExpr &t) = 0;
  virtual T visit(Throw &t) = 0;
  virtual T visit(TypeCast &tc) = 0;
  virtual T visit(TypeDefDecl &tc) = 0;
  virtual T visit(ValueAt &va) = 0;
  virtual T visit(VarDecl &vd) = 0;
  virtual T visit(VarDef &vd) = 0;
  virtual T visit(VarExpr &ve) = 0;
  virtual T visit(While &w) = 0;
};

} // namespace ACC
