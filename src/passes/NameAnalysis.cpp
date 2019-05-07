#include "passes/NameAnalysis.h"

#include "atl/include/set.h"

using namespace ACC;

NameAnalysis::NameAnalysis(atl::shared_ptr<Program> progAST)
    : progAST(progAST), inClassTypeDef(false) {}

void NameAnalysis::error(const atl::string &error) {
  errorCount++;
  errors.push_back(error);
}

void NameAnalysis::printErrors() {
  printf("Name Analysis Errors:\n");
  for (unsigned int idx = 0; idx < errors.size(); ++idx)
    printf("\t%s\n", errors[idx].c_str());
}

void NameAnalysis::run() { visit(*progAST); }

/* ---- Visit AST ---- */

void NameAnalysis::visit(AddressOf &ao) { ao.addressOfExpr->accept(*this); }
void NameAnalysis::visit(Allocation &a) {
  if (a.varConstructorCall)
    a.varConstructorCall->accept(*this);
}
void NameAnalysis::visit(ArrayAccess &aa) { aa.array->accept(*this); }
void NameAnalysis::visit(ArrayType &at) {}
void NameAnalysis::visit(Assign &as) {
  as.lhs->accept(*this);
  as.rhs->accept(*this);
}
void NameAnalysis::visit(BaseType &bt) {}
void NameAnalysis::visit(BinOp &bo) {
  bo.lhs->accept(*this);
  bo.rhs->accept(*this);
}
void NameAnalysis::visit(Block &b) {
  b.outerScope = currScope;
  currScope = b.getptr();

  for (unsigned int idx = 0; idx < b.stmts.size(); ++idx)
    b.stmts[idx]->accept(*this);

  currScope = b.outerScope;
}
void NameAnalysis::visit(BoolLiteral &bl) {}
void NameAnalysis::visit(CharLiteral &cl) {}
void NameAnalysis::visit(ClassType &ct) {}
void NameAnalysis::visit(ClassTypeDecl &ctd) {
  if (currScope->duplicateDeclaration(ctd.getptr()))
    return error("Attempted to declare a Class with an identifier that is "
                 "already in use: " +
                 ctd.getIdentifier()->toString());
}
void NameAnalysis::visit(ClassTypeDef &ctd) {
  if (currScope->duplicateDeclaration(ctd.getptr()))
    return error("Attempted to declare a Class with an identifier that is "
                 "already in use: " +
                 ctd.getIdentifier()->toString());
  currScope->insertDecl(ctd.getptr());

  ctd.outerScope = currScope;
  currScope = ctd.getptr();

  inClassTypeDef = true;
  for (unsigned int idx = 0; idx < ctd.classDecls.size(); ++idx)
    ctd.classDecls[idx]->accept(*this);
  inClassTypeDef = false;

  currScope = ctd.outerScope;
}
void NameAnalysis::visit(ConstructorDecl &cd) {
  cd.outerScope = currScope;
  currScope = cd.getptr();

  for (unsigned int idx = 0; idx < cd.constructorParams.size(); ++idx)
    cd.constructorParams[idx]->accept(*this);

  currScope = cd.outerScope;
}
void NameAnalysis::visit(ConstructorDef &cd) {
  cd.outerScope = currScope;
  currScope = cd.getptr();

  for (unsigned int idx = 0; idx < cd.constructorParams.size(); ++idx)
    cd.constructorParams[idx]->accept(*this);
  cd.constructorBlock->accept(*this);

  currScope = cd.outerScope;
}
void NameAnalysis::visit(Deletion &d) { d.deletionVar->accept(*this); }
void NameAnalysis::visit(DestructorDecl &dd) {}
void NameAnalysis::visit(DestructorDef &dd) {
  dd.destructorBlock->accept(*this);
}
void NameAnalysis::visit(DoWhile &dw) {
  dw.condition->accept(*this);
  dw.body->accept(*this);
}
void NameAnalysis::visit(EnumClassTypeDecl &ectd) {
  // TODO:
}
void NameAnalysis::visit(For &f) {
  f.outerScope = currScope;
  currScope = f.getptr();

  f.initialVarDecl->accept(*this);
  f.condition->accept(*this);
  f.endBodyExpr->accept(*this);
  f.body->accept(*this);

  currScope = f.outerScope;
}
void NameAnalysis::visit(FunCall &fc) {
  // Resolve the FunDecl/FunDef in TypeAnalysis.
  for (unsigned int idx = 0; idx < fc.funArgs.size(); ++idx)
    fc.funArgs[idx]->accept(*this);
}
void NameAnalysis::visit(FunDecl &fd) {
  // Resolve the FunDecl Signature in TypeAnalysis.
  if (!inClassTypeDef)
    currScope->insertDecl(fd.getptr());

  for (unsigned int idx = 0; idx < fd.funParams.size(); ++idx)
    fd.funParams[idx]->accept(*this);
}
void NameAnalysis::visit(FunDef &fd) {
  // Resolve the FunDef Signature in TypeAnalysis.
  if (!inClassTypeDef)
    currScope->insertDecl(fd.getptr());

  fd.outerScope = currScope;
  currScope = fd.getptr();

  for (unsigned int idx = 0; idx < fd.funParams.size(); ++idx)
    fd.funParams[idx]->accept(*this);
  fd.funBlock->accept(*this);

  currScope = fd.outerScope;
}
void NameAnalysis::visit(If &i) {
  i.ifCondition->accept(*this);
  i.ifBody->accept(*this);
  if (i.elseBody)
    i.elseBody->accept(*this);
}
void NameAnalysis::visit(IntLiteral &il) {}
void NameAnalysis::visit(MemberAccess &ma) { ma.object->accept(*this); }
void NameAnalysis::visit(MemberCall &mc) {
  mc.object->accept(*this);
  mc.funCall->accept(*this);
}
void NameAnalysis::visit(Namespace &n) {
  n.outerScope = currScope;
  currScope = n.getptr();

  for (unsigned int i = 0; i < n.namespaceDecls.size(); ++i)
    n.namespaceDecls[i]->accept(*this);

  currScope = n.outerScope;
}

void NameAnalysis::visit(Nullptr &n) {}
void NameAnalysis::visit(ParenthExpr &pe) { pe.innerExpr->accept(*this); }
void NameAnalysis::visit(PointerType &pt) {}
void NameAnalysis::visit(PrefixOp &po) { po.variable->accept(*this); }
void NameAnalysis::visit(Program &p) {
  currScope = atl::make_shared<Block>(Block({}));

  for (unsigned int idx = 0; idx < p.decls.size(); ++idx)
    p.decls[idx]->accept(*this);

  p.globalScope = currScope;
}
void NameAnalysis::visit(ReferenceType &rt) {
  rt.referencedType->accept(*this);
}
void NameAnalysis::visit(Return &r) {
  if (r.returnExpr)
    r.returnExpr->accept(*this);
}
void NameAnalysis::visit(SizeOf &so) {}
void NameAnalysis::visit(StringLiteral &sl) {}
void NameAnalysis::visit(StructType &st) {}
void NameAnalysis::visit(StructTypeDecl &std) {
  if (currScope->duplicateDeclarationLocal(std.getptr()))
    return error("Attempted to declare a Struct with an identifier that is "
                 "already in use: " +
                 std.getIdentifier()->toString());

  currScope->insertDecl(std.getptr());

  std.outerScope = currScope;
  currScope = std.getptr();

  /* Check that the fields in this struct are unique */
  for (unsigned int idx = 0; idx < std.varDecls.size(); ++idx) {
    if (currScope->duplicateDeclarationLocal(std.varDecls[idx]))
      return error("Struct " + std.getIdentifier()->toString() +
                   " contained multiple fields with the same identifier: " +
                   std.varDecls[idx]->getIdentifier()->toString());
    std.varDecls[idx]->accept(*this);
  }

  currScope = std.outerScope;
}
void NameAnalysis::visit(TertiaryExpr &t) {
  t.tertiaryCondition->accept(*this);
  t.tertiaryIfBody->accept(*this);
  t.tertiaryElseBody->accept(*this);
}
void NameAnalysis::visit(Throw &t) {}
void NameAnalysis::visit(TypeCast &tc) { tc.expr->accept(*this); }
void NameAnalysis::visit(TypeDefDecl &tdd) {
  // TODO:
}
void NameAnalysis::visit(ValueAt &va) { va.derefExpr->accept(*this); }
void NameAnalysis::visit(VarDecl &vd) {
  if (!inClassTypeDef && currScope->duplicateDeclarationLocal(vd.getptr()))
    return error("Attempted to declare a Variable with an identifier that is "
                 "already in use: " +
                 vd.getIdentifier()->toString());
  currScope->insertDecl(vd.getptr());
}
void NameAnalysis::visit(VarDef &vd) {
  if (!inClassTypeDef && currScope->duplicateDeclarationLocal(vd.getptr()))
    return error("Attempted to define a Variable with an identifier that is "
                 "already in use: " +
                 vd.getIdentifier()->toString());
  currScope->insertDecl(vd.getptr());
}
void NameAnalysis::visit(VarExpr &ve) {
  const atl::shared_ptr<VarDecl> varDecl =
      currScope->resolveVarExpr(ve.varIdentifier);
  if (varDecl == nullptr)
    return error("Attempted to reference undeclared variable: " +
                 ve.varIdentifier->toString());
  ve.varDecl = varDecl;
}
void NameAnalysis::visit(While &w) {
  w.condition->accept(*this);
  w.body->accept(*this);
}