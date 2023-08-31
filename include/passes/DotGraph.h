#pragma once

#include "AST.h"
#include "ASTVisitor.h"

namespace ACC {

class DotGraph : public ASTVisitor<atl::string> {

public:
  DotGraph(atl::shared_ptr<Program> progAST, const atl::string &outputFilename);
  void print();

private:
  int nodeCount = 0;
  atl::ofstream outputFile;
  atl::shared_ptr<Program> progAST;
  atl::unordered_map<atl::string, atl::string> classTypeDeclIDs;

  void declare(const atl::string &nodeID, const atl::string &label);

  void join(const atl::string &lhs, const atl::string &rhs);

  void write(const atl::string &str);

  /* ---- Visit AST ---- */

  atl::string visit(AddressOf &ao) override;
  atl::string visit(Allocation &a) override;
  atl::string visit(ArrayType &at) override;
  atl::string visit(Assign &as) override;
  atl::string visit(BaseType &bt) override;
  atl::string visit(BinOp &bo) override;
  atl::string visit(Block &b) override;
  atl::string visit(BoolLiteral &bl) override;
  atl::string visit(CharLiteral &cl) override;
  atl::string visit(ClassType &ct) override;
  atl::string visit(ClassTypeDecl &ctd) override;
  atl::string visit(ClassTypeDef &ctd) override;
  atl::string visit(ConstructorCall &cc) override;
  atl::string visit(ConstructorDecl &cd) override;
  atl::string visit(ConstructorDef &cd) override;
  atl::string visit(Deletion &d) override;
  atl::string visit(DestructorDecl &dd) override;
  atl::string visit(DestructorDef &dd) override;
  atl::string visit(DoWhile &dw) override;
  atl::string visit(EnumClassTypeDecl &ectd) override;
  atl::string visit(For &f) override;
  atl::string visit(FunCall &fc) override;
  atl::string visit(FunDecl &fd) override;
  atl::string visit(FunDef &fd) override;
  atl::string visit(Identifier &i) override;
  atl::string visit(If &i) override;
  atl::string visit(IntLiteral &il) override;
  atl::string visit(MemberAccess &ma) override;
  atl::string visit(MemberCall &mc) override;
  atl::string visit(Namespace &n) override;
  atl::string visit(Not &n) override;
  atl::string visit(Nullptr &n) override;
  atl::string visit(ParenthExpr &pe) override;
  atl::string visit(PointerType &pt) override;
  atl::string visit(PrefixOp &pi) override;
  atl::string visit(Program &p) override;
  atl::string visit(ReferenceType &rt) override;
  atl::string visit(Return &r) override;
  atl::string visit(SizeOf &so) override;
  atl::string visit(StaticCast &sc) override;
  atl::string visit(StringLiteral &sl) override;
  atl::string visit(SubscriptOp &so) override;
  atl::string visit(TemplateDef &td) override;
  atl::string visit(TemplatedFunCall &tfc) override;
  atl::string visit(TertiaryExpr &t) override;
  atl::string visit(Throw &t) override;
  atl::string visit(TypeDefDecl &td) override;
  atl::string visit(ValueAt &va) override;
  atl::string visit(VarDecl &vd) override;
  atl::string visit(VarDef &vd) override;
  atl::string visit(VarExpr &ve) override;
  atl::string visit(While &w) override;
};

} // namespace ACC
