#include "passes/SemanticAnalysis.h"

#include "atl/include/set.h"

using namespace ACC;

SemanticAnalysis::SemanticAnalysis(atl::shared_ptr<Program> progAST)
    : progAST(progAST), inClassTypeDef(false) {}

void SemanticAnalysis::error(const atl::string &error,
                             const atl::shared_ptr<ASTNode> &node) {
  errorCount++;
  errors.push_back("Semantic Analysis Error at: " + node->position.toString() +
                   "\n\t" + error);
}

void SemanticAnalysis::printErrors() {
  printf("Name Analysis Errors:\n");
  for (unsigned int idx = 0; idx < errors.size(); ++idx)
    printf("\t%s\n", errors[idx].c_str());
}

void SemanticAnalysis::run() { visit(*progAST); }

/* ---- Visit AST ---- */

void SemanticAnalysis::visit(AddressOf &ao) { ao.addressOfExpr->accept(*this); }
void SemanticAnalysis::visit(Allocation &a) {
  if (a.varConstructorCall)
    a.varConstructorCall->accept(*this);
}
void SemanticAnalysis::visit(ArrayAccess &aa) { aa.array->accept(*this); }
void SemanticAnalysis::visit(ArrayType &at) {}
void SemanticAnalysis::visit(Assign &as) {
  as.lhs->accept(*this);
  as.rhs->accept(*this);
}
void SemanticAnalysis::visit(BaseType &bt) {}
void SemanticAnalysis::visit(BinOp &bo) {
  bo.lhs->accept(*this);
  bo.rhs->accept(*this);
}
void SemanticAnalysis::visit(Block &b) {
  b.outerScope = currScope;
  currScope = b.getptr();

  for (unsigned int idx = 0; idx < b.stmts.size(); ++idx)
    b.stmts[idx]->accept(*this);

  currScope = b.outerScope;
}
void SemanticAnalysis::visit(BoolLiteral &bl) {}
void SemanticAnalysis::visit(CharLiteral &cl) {}
void SemanticAnalysis::visit(ClassType &ct) {}
void SemanticAnalysis::visit(ClassTypeDecl &ctd) {
  if (currScope->duplicateDeclaration(ctd.getptr()))
    return error("Attempted to declare a Class with an identifier that is "
                 "already in use: " +
                     ctd.getIdentifier()->toString(),
                 ctd.getptr());
}
void SemanticAnalysis::visit(ClassTypeDef &ctd) {
  if (currScope->duplicateDeclaration(ctd.getptr()))
    return error("Attempted to declare a Class with an identifier that is "
                 "already in use: " +
                     ctd.getIdentifier()->toString(),
                 ctd.getptr());
  currScope->insertDecl(ctd.getptr());

  ctd.outerScope = currScope;
  currScope = ctd.getptr();

  inClassTypeDef = true;
  for (unsigned int idx = 0; idx < ctd.classDecls.size(); ++idx)
    ctd.classDecls[idx]->accept(*this);
  inClassTypeDef = false;

  currScope = ctd.outerScope;
}
void SemanticAnalysis::visit(ConstructorDecl &cd) {
  cd.outerScope = currScope;
  currScope = cd.getptr();

  for (unsigned int idx = 0; idx < cd.constructorParams.size(); ++idx)
    cd.constructorParams[idx]->accept(*this);

  currScope = cd.outerScope;
}
void SemanticAnalysis::visit(ConstructorDef &cd) {
  cd.outerScope = currScope;
  currScope = cd.getptr();

  for (unsigned int idx = 0; idx < cd.constructorParams.size(); ++idx)
    cd.constructorParams[idx]->accept(*this);
  cd.constructorBlock->accept(*this);

  currScope = cd.outerScope;
}
void SemanticAnalysis::visit(Deletion &d) { d.deletionVar->accept(*this); }
void SemanticAnalysis::visit(DestructorDecl &dd) {}
void SemanticAnalysis::visit(DestructorDef &dd) {
  dd.destructorBlock->accept(*this);
}
void SemanticAnalysis::visit(DoWhile &dw) {
  dw.condition->accept(*this);
  dw.body->accept(*this);
}
void SemanticAnalysis::visit(EnumClassTypeDecl &ectd) {
  // TODO:
}
void SemanticAnalysis::visit(For &f) {
  f.outerScope = currScope;
  currScope = f.getptr();

  f.initialVarDecl->accept(*this);
  f.condition->accept(*this);
  f.endBodyExpr->accept(*this);
  f.body->accept(*this);

  currScope = f.outerScope;
}
void SemanticAnalysis::visit(FunCall &fc) {
  // Resolve the FunDecl/FunDef in TypeAnalysis.
  for (unsigned int idx = 0; idx < fc.funArgs.size(); ++idx)
    fc.funArgs[idx]->accept(*this);
}
void SemanticAnalysis::visit(FunDecl &fd) {
  // Resolve the FunDecl Signature in TypeAnalysis.
  if (!inClassTypeDef)
    currScope->insertDecl(fd.getptr());

  for (unsigned int idx = 0; idx < fd.funParams.size(); ++idx)
    fd.funParams[idx]->accept(*this);
}
void SemanticAnalysis::visit(FunDef &fd) {
  const atl::shared_ptr<Decl> existingDecl =
      currScope->findVarDecl(fd.getIdentifier());
  if (existingDecl != nullptr)
    return error("Name Analysis: FunDef Identifier already in use: " +
                     fd.getIdentifier()->toString(),
                 fd.getptr());
  currScope->insertDecl(fd.getptr());

  fd.outerScope = currScope;
  currScope = fd.getptr();

  for (unsigned int idx = 0; idx < fd.funParams.size(); ++idx)
    fd.funParams[idx]->accept(*this);
  fd.funBlock->accept(*this);

  currScope = fd.outerScope;
}
void SemanticAnalysis::visit(Identifier &i) {
  // TODO
}
void SemanticAnalysis::visit(If &i) {
  i.ifCondition->accept(*this);
  i.ifBody->accept(*this);
  if (i.elseBody)
    i.elseBody->accept(*this);
}
void SemanticAnalysis::visit(IntLiteral &il) {}
void SemanticAnalysis::visit(MemberAccess &ma) { ma.object->accept(*this); }
void SemanticAnalysis::visit(MemberCall &mc) {
  mc.object->accept(*this);
  mc.funCall->accept(*this);
}
void SemanticAnalysis::visit(Namespace &n) {
  n.outerScope = currScope;
  currScope = n.getptr();

  for (unsigned int i = 0; i < n.namespaceDecls.size(); ++i)
    n.namespaceDecls[i]->accept(*this);

  currScope = n.outerScope;
}

void SemanticAnalysis::visit(Nullptr &n) {}
void SemanticAnalysis::visit(ParenthExpr &pe) { pe.innerExpr->accept(*this); }
void SemanticAnalysis::visit(PointerType &pt) {}
void SemanticAnalysis::visit(PrefixOp &po) { po.variable->accept(*this); }
void SemanticAnalysis::visit(Program &p) {
  currScope = atl::make_shared<Block>(Block({}));

  for (unsigned int idx = 0; idx < p.decls.size(); ++idx)
    p.decls[idx]->accept(*this);

  p.globalScope = currScope;
}
void SemanticAnalysis::visit(ReferenceType &rt) {
  rt.referencedType->accept(*this);
}
void SemanticAnalysis::visit(Return &r) {
  if (r.returnExpr)
    r.returnExpr->accept(*this);
}
void SemanticAnalysis::visit(SizeOf &so) {}
void SemanticAnalysis::visit(StringLiteral &sl) {}
void SemanticAnalysis::visit(StructType &st) {}
void SemanticAnalysis::visit(StructTypeDecl &std) {
  if (currScope->duplicateDeclarationLocal(std.getptr()))
    return error("Attempted to declare a Struct with an identifier that is "
                 "already in use: " +
                     std.getIdentifier()->toString(),
                 std.getptr());

  currScope->insertDecl(std.getptr());

  std.outerScope = currScope;
  currScope = std.getptr();

  /* Check that the fields in this struct are unique */
  for (unsigned int idx = 0; idx < std.varDecls.size(); ++idx) {
    if (currScope->duplicateDeclarationLocal(std.varDecls[idx]))
      return error("Struct " + std.getIdentifier()->toString() +
                       " contained multiple fields with the same identifier: " +
                       std.varDecls[idx]->getIdentifier()->toString(),
                   atl::static_pointer_cast<Decl>(std.varDecls[idx]));
    std.varDecls[idx]->accept(*this);
  }

  currScope = std.outerScope;
}
void SemanticAnalysis::visit(TertiaryExpr &t) {
  t.tertiaryCondition->accept(*this);
  t.tertiaryIfBody->accept(*this);
  t.tertiaryElseBody->accept(*this);
}
void SemanticAnalysis::visit(Throw &t) {}
void SemanticAnalysis::visit(TypeCast &tc) { tc.expr->accept(*this); }
void SemanticAnalysis::visit(TypeDefDecl &tdd) {
  // TODO:
}
void SemanticAnalysis::visit(ValueAt &va) { va.derefExpr->accept(*this); }
void SemanticAnalysis::visit(VarDecl &vd) {
  if (currScope->findVarDecl(vd.getIdentifier()))
    return error("Name Analysis: Attempted to declare a Variable with an "
                 "identifier that is already in use: " +
                     vd.getIdentifier()->toString(),
                 atl::static_pointer_cast<Decl>(vd.getptr()));
  currScope->insertDecl(vd.getptr());
}
void SemanticAnalysis::visit(VarDef &vd) {
  if (currScope->findVarDecl(vd.getIdentifier()))
    return error("Name Analysis: Attempted to define a Variable with an "
                 "identifier that is already in use: " +
                     vd.getIdentifier()->toString(),
                 atl::static_pointer_cast<Decl>(vd.getptr()));
  currScope->insertDecl(vd.getptr());
}
void SemanticAnalysis::visit(VarExpr &ve) {
  const atl::shared_ptr<VarDecl> varDecl =
      currScope->resolveVarExpr(ve.varIdentifier);
  if (varDecl == nullptr)
    return error("Attempted to reference undeclared variable: " +
                     ve.varIdentifier->toString(),
                 ve.getptr());
  ve.varDecl = varDecl;
}
void SemanticAnalysis::visit(While &w) {
  w.condition->accept(*this);
  w.body->accept(*this);
}