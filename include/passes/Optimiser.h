#pragma once

#include "AST.h"
#include "ASTVisitor.h"

namespace ACC {

class Optimiser : public ASTVisitor<atl::shared_ptr<ASTNode>> {

public:
  int optimisationsCount = 0;
  atl::vector<atl::string> optimisations;

  Optimiser(atl::shared_ptr<Program> &progAST);

  void optimised(const atl::string &message,
                 const atl::shared_ptr<ASTNode> &node);

  void printOptimisations();

  void run();

private:
  atl::shared_ptr<Program> &progAST;

  atl::shared_ptr<Scope> currScope;

  /* ---- Visit AST ---- */

  atl::shared_ptr<ASTNode> visit(AddressOf &ao) override;
  atl::shared_ptr<ASTNode> visit(Allocation &a) override;
  atl::shared_ptr<ASTNode> visit(ArrayType &at) override;
  atl::shared_ptr<ASTNode> visit(Assign &as) override;
  atl::shared_ptr<ASTNode> visit(BaseType &bt) override;
  atl::shared_ptr<ASTNode> visit(BinOp &bo) override;
  atl::shared_ptr<ASTNode> visit(Block &b) override;
  atl::shared_ptr<ASTNode> visit(BoolLiteral &bl) override;
  atl::shared_ptr<ASTNode> visit(CharLiteral &cl) override;
  atl::shared_ptr<ASTNode> visit(ClassType &ct) override;
  atl::shared_ptr<ASTNode> visit(ClassTypeDecl &ctd) override;
  atl::shared_ptr<ASTNode> visit(ClassTypeDef &ctd) override;
  atl::shared_ptr<ASTNode> visit(ConstructorCall &cc) override;
  atl::shared_ptr<ASTNode> visit(ConstructorDecl &cd) override;
  atl::shared_ptr<ASTNode> visit(ConstructorDef &cd) override;
  atl::shared_ptr<ASTNode> visit(Deletion &d) override;
  atl::shared_ptr<ASTNode> visit(DestructorDecl &dd) override;
  atl::shared_ptr<ASTNode> visit(DestructorDef &dd) override;
  atl::shared_ptr<ASTNode> visit(DoWhile &dw) override;
  atl::shared_ptr<ASTNode> visit(EnumClassTypeDecl &ectd) override;
  atl::shared_ptr<ASTNode> visit(For &f) override;
  atl::shared_ptr<ASTNode> visit(FunCall &fc) override;
  atl::shared_ptr<ASTNode> visit(FunDecl &fd) override;
  atl::shared_ptr<ASTNode> visit(FunDef &fd) override;
  atl::shared_ptr<ASTNode> visit(Identifier &i) override;
  atl::shared_ptr<ASTNode> visit(If &i) override;
  atl::shared_ptr<ASTNode> visit(IntLiteral &il) override;
  atl::shared_ptr<ASTNode> visit(MemberAccess &ma) override;
  atl::shared_ptr<ASTNode> visit(MemberCall &mc) override;
  atl::shared_ptr<ASTNode> visit(Namespace &n) override;
  atl::shared_ptr<ASTNode> visit(Not &n) override;
  atl::shared_ptr<ASTNode> visit(Nullptr &n) override;
  atl::shared_ptr<ASTNode> visit(ParenthExpr &pe) override;
  atl::shared_ptr<ASTNode> visit(PointerType &pt) override;
  atl::shared_ptr<ASTNode> visit(PrefixOp &pi) override;
  atl::shared_ptr<ASTNode> visit(Program &p) override;
  atl::shared_ptr<ASTNode> visit(ReferenceType &rt) override;
  atl::shared_ptr<ASTNode> visit(Return &r) override;
  atl::shared_ptr<ASTNode> visit(SizeOf &so) override;
  atl::shared_ptr<ASTNode> visit(StaticCast &sc) override;
  atl::shared_ptr<ASTNode> visit(StringLiteral &sl) override;
  atl::shared_ptr<ASTNode> visit(SubscriptOp &so) override;
  atl::shared_ptr<ASTNode> visit(TemplateDef &td) override;
  atl::shared_ptr<ASTNode> visit(TemplatedFunCall &tfc) override;
  atl::shared_ptr<ASTNode> visit(TertiaryExpr &t) override;
  atl::shared_ptr<ASTNode> visit(Throw &t) override;
  atl::shared_ptr<ASTNode> visit(TypeDefDecl &td) override;
  atl::shared_ptr<ASTNode> visit(ValueAt &va) override;
  atl::shared_ptr<ASTNode> visit(VarDecl &vd) override;
  atl::shared_ptr<ASTNode> visit(VarDef &vd) override;
  atl::shared_ptr<ASTNode> visit(VarExpr &ve) override;
  atl::shared_ptr<ASTNode> visit(While &w) override;
};

} // namespace ACC
