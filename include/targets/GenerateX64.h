#pragma once

#include "atl/include/string.h"

#include "AST.h"
#include "ASTVisitor.h"
#include "targets/X64.h"

namespace ACC {

class GenerateX64 : public ASTVisitor<atl::shared_ptr<X64::Operand>> {

public:
  int errorCount = 0;
  atl::vector<atl::string> errors;

  GenerateX64(atl::shared_ptr<Program> progAST, const atl::string &outputFile);

  void printErrors() const;
  void run();

private:
  void error(atl::string error);

  atl::shared_ptr<Program> progAST;

  atl::shared_ptr<Scope> currScope;
  atl::shared_ptr<VarExpr> currObject;

  int blockCount = 0;
  int currBpOffset = 0;
  int stringCount = 0;

  /* ---- X64 Memory ---- */
  X64::Writer x64;

  void alloc(const VarDecl &vd);
  void declareExternFuncs();
  void defSystemFunDecls();
  void mainEntry();

  /* ---- Visit AST ---- */

  atl::shared_ptr<X64::Operand> visit(AddressOf &ao) override;
  atl::shared_ptr<X64::Operand> visit(Allocation &a) override;
  atl::shared_ptr<X64::Operand> visit(ArrayType &at) override;
  atl::shared_ptr<X64::Operand> visit(Assign &as) override;
  atl::shared_ptr<X64::Operand> visit(BaseType &bt) override;
  atl::shared_ptr<X64::Operand> visit(BinOp &bo) override;
  atl::shared_ptr<X64::Operand> visit(Block &b) override;
  atl::shared_ptr<X64::Operand> visit(BoolLiteral &bl) override;
  atl::shared_ptr<X64::Operand> visit(CharLiteral &cl) override;
  atl::shared_ptr<X64::Operand> visit(ClassType &ct) override;
  atl::shared_ptr<X64::Operand> visit(ClassTypeDecl &ctd) override;
  atl::shared_ptr<X64::Operand> visit(ClassTypeDef &ctd) override;
  atl::shared_ptr<X64::Operand> visit(ConstructorCall &cc) override;
  atl::shared_ptr<X64::Operand> visit(ConstructorDecl &cd) override;
  atl::shared_ptr<X64::Operand> visit(ConstructorDef &cd) override;
  atl::shared_ptr<X64::Operand> visit(Deletion &d) override;
  atl::shared_ptr<X64::Operand> visit(DestructorDecl &dd) override;
  atl::shared_ptr<X64::Operand> visit(DestructorDef &dd) override;
  atl::shared_ptr<X64::Operand> visit(DoWhile &dw) override;
  atl::shared_ptr<X64::Operand> visit(EnumClassTypeDecl &ectd) override;
  atl::shared_ptr<X64::Operand> visit(For &f) override;
  atl::shared_ptr<X64::Operand> visit(FunCall &fc) override;
  atl::shared_ptr<X64::Operand> visit(FunDecl &fd) override;
  atl::shared_ptr<X64::Operand> visit(FunDef &fd) override;
  atl::shared_ptr<X64::Operand> visit(Identifier &i) override;
  atl::shared_ptr<X64::Operand> visit(If &i) override;
  atl::shared_ptr<X64::Operand> visit(IntLiteral &il) override;
  atl::shared_ptr<X64::Operand> visit(MemberAccess &ma) override;
  atl::shared_ptr<X64::Operand> visit(MemberCall &mc) override;
  atl::shared_ptr<X64::Operand> visit(Namespace &n) override;
  atl::shared_ptr<X64::Operand> visit(Nullptr &n) override;
  atl::shared_ptr<X64::Operand> visit(ParenthExpr &pe) override;
  atl::shared_ptr<X64::Operand> visit(PointerType &pt) override;
  atl::shared_ptr<X64::Operand> visit(PrefixOp &pi) override;
  atl::shared_ptr<X64::Operand> visit(Program &p) override;
  atl::shared_ptr<X64::Operand> visit(ReferenceType &rt) override;
  atl::shared_ptr<X64::Operand> visit(Return &r) override;
  atl::shared_ptr<X64::Operand> visit(SizeOf &so) override;
  atl::shared_ptr<X64::Operand> visit(StaticCast &sc) override;
  atl::shared_ptr<X64::Operand> visit(StringLiteral &sl) override;
  atl::shared_ptr<X64::Operand> visit(SubscriptOp &so) override;
  atl::shared_ptr<X64::Operand> visit(TemplateDef &td) override;
  atl::shared_ptr<X64::Operand> visit(TertiaryExpr &t) override;
  atl::shared_ptr<X64::Operand> visit(Throw &t) override;
  atl::shared_ptr<X64::Operand> visit(TypeDefDecl &td) override;
  atl::shared_ptr<X64::Operand> visit(ValueAt &va) override;
  atl::shared_ptr<X64::Operand> visit(VarDecl &vd) override;
  atl::shared_ptr<X64::Operand> visit(VarDef &vd) override;
  atl::shared_ptr<X64::Operand> visit(VarExpr &ve) override;
  atl::shared_ptr<X64::Operand> visit(While &w) override;

  /* ---- Helpers ---- */
  atl::shared_ptr<X64::AddrOffset> addrOffset(const atl::shared_ptr<X64::Operand> addrOperand, const int offset);
  atl::shared_ptr<X64::IntValue> genIntValue(int value);
};
} // namespace ACC
