#pragma once

#include "AST.h"
#include "ASTVisitor.h"

namespace ACC {

class SemanticAnalysis : public ASTVisitor<atl::shared_ptr<Type>> {

public:
  int errorCount = 0;
  atl::vector<atl::string> errors;

  SemanticAnalysis(atl::shared_ptr<Program> progAST);

  atl::shared_ptr<Type> error(const atl::string &errorType,
                              const atl::string &error,
                              const atl::shared_ptr<ASTNode> &node);

  void printErrors();

  void run();

private:
  atl::shared_ptr<Program> progAST;
  atl::shared_ptr<Scope> currScope;

  // Too lazy to make MemberFunDecl/MemberFunDef ASTNodes
  bool inClassTypeDef;

  /* ---- Visit AST ---- */

  atl::shared_ptr<Type> visit(AddressOf &ao) override;
  atl::shared_ptr<Type> visit(Allocation &a) override;
  atl::shared_ptr<Type> visit(ArrayAccess &aa) override;
  atl::shared_ptr<Type> visit(ArrayType &at) override;
  atl::shared_ptr<Type> visit(Assign &as) override;
  atl::shared_ptr<Type> visit(BaseType &bt) override;
  atl::shared_ptr<Type> visit(BinOp &bo) override;
  atl::shared_ptr<Type> visit(Block &b) override;
  atl::shared_ptr<Type> visit(BoolLiteral &bl) override;
  atl::shared_ptr<Type> visit(CharLiteral &cl) override;
  atl::shared_ptr<Type> visit(ClassType &ct) override;
  atl::shared_ptr<Type> visit(ClassTypeDecl &ctd) override;
  atl::shared_ptr<Type> visit(ClassTypeDef &ctd) override;
  atl::shared_ptr<Type> visit(ConstructorDecl &cd) override;
  atl::shared_ptr<Type> visit(ConstructorDef &cd) override;
  atl::shared_ptr<Type> visit(Deletion &d) override;
  atl::shared_ptr<Type> visit(DestructorDecl &cd) override;
  atl::shared_ptr<Type> visit(DestructorDef &dd) override;
  atl::shared_ptr<Type> visit(DoWhile &dw) override;
  atl::shared_ptr<Type> visit(EnumClassTypeDecl &ectd) override;
  atl::shared_ptr<Type> visit(For &f) override;
  atl::shared_ptr<Type> visit(FunCall &fc) override;
  atl::shared_ptr<Type> visit(FunDecl &fd) override;
  atl::shared_ptr<Type> visit(FunDef &fd) override;
  atl::shared_ptr<Type> visit(Identifier &i) override;
  atl::shared_ptr<Type> visit(If &i) override;
  atl::shared_ptr<Type> visit(IntLiteral &il) override;
  atl::shared_ptr<Type> visit(MemberAccess &ma) override;
  atl::shared_ptr<Type> visit(MemberCall &mc) override;
  atl::shared_ptr<Type> visit(Namespace &n) override;
  atl::shared_ptr<Type> visit(Nullptr &n) override;
  atl::shared_ptr<Type> visit(ParenthExpr &pe) override;
  atl::shared_ptr<Type> visit(PointerType &pt) override;
  atl::shared_ptr<Type> visit(PrefixOp &po) override;
  atl::shared_ptr<Type> visit(Program &p) override;
  atl::shared_ptr<Type> visit(ReferenceType &rt) override;
  atl::shared_ptr<Type> visit(Return &r) override;
  atl::shared_ptr<Type> visit(SizeOf &so) override;
  atl::shared_ptr<Type> visit(StringLiteral &sl) override;
  atl::shared_ptr<Type> visit(SubscriptOp &so) override;
  atl::shared_ptr<Type> visit(TertiaryExpr &t) override;
  atl::shared_ptr<Type> visit(Throw &t) override;
  atl::shared_ptr<Type> visit(TypeCast &tc) override;
  atl::shared_ptr<Type> visit(TypeDefDecl &td) override;
  atl::shared_ptr<Type> visit(ValueAt &va) override;
  atl::shared_ptr<Type> visit(VarDecl &vd) override;
  atl::shared_ptr<Type> visit(VarDef &vd) override;
  atl::shared_ptr<Type> visit(VarExpr &ve) override;
  atl::shared_ptr<Type> visit(While &w) override;
};

} // namespace ACC
