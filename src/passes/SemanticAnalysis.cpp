#include "passes/SemanticAnalysis.h"

#include "atl/include/set.h"

using namespace ACC;

SemanticAnalysis::SemanticAnalysis(atl::shared_ptr<Program> progAST)
    : progAST(progAST) {}

atl::shared_ptr<Type>
SemanticAnalysis::error(const atl::string &errorType, const atl::string &error,
                        const atl::shared_ptr<ASTNode> &node) {
  errorCount++;
  errors.push_back(errorType + " Error at: " + node->position.toString() +
                   "\n\t" + error);
  return atl::shared_ptr<BaseType>(new BaseType(PrimitiveType::NULLPTR_T));
}

void SemanticAnalysis::printErrors() {
  printf("Semantic Analysis Errors:\n");
  for (unsigned int idx = 0; idx < errors.size(); ++idx)
    printf("\t%s\n", errors[idx].c_str());
}

void SemanticAnalysis::run() { visit(*progAST); }

/* ---- Visit AST ---- */

atl::shared_ptr<Type> SemanticAnalysis::visit(AddressOf &ao) {
  return atl::make_shared<PointerType>(
      PointerType(ao.addressOfExpr->accept(*this)));
}
atl::shared_ptr<Type> SemanticAnalysis::visit(Allocation &a) {
  // TODO
  return atl::shared_ptr<BaseType>(new BaseType(PrimitiveType::NULLPTR_T));
}
atl::shared_ptr<Type> SemanticAnalysis::visit(ArrayAccess &aa) {
  const atl::shared_ptr<Type> arrayExprType = aa.array->accept(*this);
  const atl::shared_ptr<Type> arrayIndex = aa.index->accept(*this);
  if (arrayExprType->astClass() != "ArrayType" &&
      arrayExprType->astClass() != "PointerType")
    return error("Type Analysis",
                 "Attempted to index an expression which was not an array. Was "
                 "of type: " +
                     arrayExprType->astClass(),
                 aa.array);
  if (arrayIndex->astClass() != "BaseType")
    return error("Type Analysis",
                 "Attempted to index an array using an expression which was "
                 "not of type int. Was of type: " +
                     arrayIndex->astClass(),
                 aa.index);

  return atl::static_pointer_cast<ArrayType>(arrayExprType)->type;
}
atl::shared_ptr<Type> SemanticAnalysis::visit(ArrayType &at) {
  return at.getptr();
}
atl::shared_ptr<Type> SemanticAnalysis::visit(Assign &as) {
  const atl::shared_ptr<Type> lhs = as.lhs->accept(*this);
  const atl::shared_ptr<Type> rhs = as.rhs->accept(*this);
  if (*lhs != *rhs)
    return error("Type Analysis", "Assignation has mismatched types.",
                 as.getptr());
  return atl::shared_ptr<BaseType>(new BaseType(PrimitiveType::NULLPTR_T));
}
atl::shared_ptr<Type> SemanticAnalysis::visit(BaseType &bt) {
  return bt.getptr();
}
atl::shared_ptr<Type> SemanticAnalysis::visit(BinOp &bo) {
  const atl::shared_ptr<Type> lhsType = bo.lhs->accept(*this);
  const atl::shared_ptr<Type> rhsType = bo.rhs->accept(*this);
  if (*lhsType != *rhsType)
    return error("Type Analysis", "Binary operation's has mismatched types.",
                 bo.getptr());

  return lhsType;
}
atl::shared_ptr<Type> SemanticAnalysis::visit(Block &b) {
  b.outerScope = currScope;
  currScope = b.getptr();

  for (unsigned int idx = 0; idx < b.stmts.size(); ++idx)
    b.stmts[idx]->accept(*this);

  currScope = b.outerScope;
  return atl::shared_ptr<BaseType>(new BaseType(PrimitiveType::NULLPTR_T));
}
atl::shared_ptr<Type> SemanticAnalysis::visit(BoolLiteral &bl) {
  return atl::shared_ptr<BaseType>(new BaseType(PrimitiveType::BOOL));
}
atl::shared_ptr<Type> SemanticAnalysis::visit(CharLiteral &cl) {
  return atl::shared_ptr<BaseType>(new BaseType(PrimitiveType::CHAR));
}
atl::shared_ptr<Type> SemanticAnalysis::visit(ClassType &ct) {
  const atl::shared_ptr<ClassTypeDecl> ctd =
      currScope->findClassDecl(ct.identifier);

  if (ctd == nullptr)
    return error("Name Analysis",
                 "Attempted to use a Class that was not declared: " +
                     ct.identifier->toString(),
                 ct.getptr());

  ct.typeDefinition = ctd;
  return ct.getptr();
}
atl::shared_ptr<Type> SemanticAnalysis::visit(ClassTypeDecl &ctd) {
  if (currScope->findClassDef(ctd.getIdentifier()))
    return error("Name Analysis",
                 "Attempted to declare a Class with an identifier that is "
                 "already in use: " +
                     ctd.getIdentifier()->toString(),
                 ctd.getptr());
  currScope->insertDecl(ctd.getptr());

  return ctd.classType;
}
atl::shared_ptr<Type> SemanticAnalysis::visit(ClassTypeDef &ctd) {
  if (currScope->findClassDef(ctd.getIdentifier()))
    return error("Name Analysis",
                 "Attempted to declare a Class with an identifier that is "
                 "already in use: " +
                     ctd.getIdentifier()->toString(),
                 ctd.getptr());
  currScope->insertDecl(ctd.getptr());

  ctd.outerScope = currScope;
  currScope = ctd.getptr();

  for (unsigned int idx = 0; idx < ctd.classDecls.size(); ++idx)
    ctd.classDecls[idx]->accept(*this);

  currScope = ctd.outerScope;

  return ctd.classType;
}
atl::shared_ptr<Type> SemanticAnalysis::visit(ConstructorDecl &cd) {
  cd.outerScope = currScope;
  currScope = cd.getptr();

  for (unsigned int idx = 0; idx < cd.constructorParams.size(); ++idx)
    cd.constructorParams[idx]->accept(*this);

  currScope = cd.outerScope;

  return cd.classType;
}
atl::shared_ptr<Type> SemanticAnalysis::visit(ConstructorDef &cd) {
  cd.outerScope = currScope;
  currScope = cd.getptr();

  for (unsigned int idx = 0; idx < cd.constructorParams.size(); ++idx)
    cd.constructorParams[idx]->accept(*this);
  cd.constructorBlock->accept(*this);

  currScope = cd.outerScope;

  return cd.classType;
}
atl::shared_ptr<Type> SemanticAnalysis::visit(Deletion &d) {
  d.deletionVar->accept(*this);
  return atl::shared_ptr<BaseType>(new BaseType(PrimitiveType::NULLPTR_T));
}
atl::shared_ptr<Type> SemanticAnalysis::visit(DestructorDecl &dd) {
  return atl::shared_ptr<BaseType>(new BaseType(PrimitiveType::NULLPTR_T));
}
atl::shared_ptr<Type> SemanticAnalysis::visit(DestructorDef &dd) {
  dd.destructorBlock->accept(*this);
  return atl::shared_ptr<BaseType>(new BaseType(PrimitiveType::NULLPTR_T));
}
atl::shared_ptr<Type> SemanticAnalysis::visit(DoWhile &dw) {
  const atl::shared_ptr<Type> conditionType = dw.condition->accept(*this);
  if (conditionType->astClass() != "BaseType")
    return error("Type Analysis", "Type of If condition is invalid.",
                 dw.condition);
  atl::shared_ptr<BaseType> conditionBaseType =
      atl::static_pointer_cast<BaseType>(conditionType);
  if (conditionBaseType->primitiveType != PrimitiveType::INT &&
      conditionBaseType->primitiveType != PrimitiveType::BOOL)
    return error("Type Analysis", "Type of If condition is not INT or BOOl.",
                 dw.condition);
  dw.body->accept(*this);
  return atl::shared_ptr<BaseType>(new BaseType(PrimitiveType::NULLPTR_T));
}
atl::shared_ptr<Type> SemanticAnalysis::visit(EnumClassTypeDecl &ectd) {
  // TODO:
  return atl::shared_ptr<BaseType>(new BaseType(PrimitiveType::NULLPTR_T));
}
atl::shared_ptr<Type> SemanticAnalysis::visit(For &f) {
  f.outerScope = currScope;
  currScope = f.getptr();

  f.initialVarDecl->accept(*this);
  const atl::shared_ptr<Type> conditionType = f.condition->accept(*this);
  if (conditionType->astClass() != "BaseType")
    return error("Type Analysis", "Type of For condition is invalid.",
                 f.condition);
  atl::shared_ptr<BaseType> conditionBaseType =
      atl::static_pointer_cast<BaseType>(conditionType);
  if (conditionBaseType->primitiveType != PrimitiveType::INT &&
      conditionBaseType->primitiveType != PrimitiveType::BOOL)
    return error("Type Analysis", "Type of For condition is not INT or BOOl.",
                 f.condition);
  f.endBodyExpr->accept(*this);
  f.body->accept(*this);

  currScope = f.outerScope;
  return atl::shared_ptr<BaseType>(new BaseType(PrimitiveType::NULLPTR_T));
}
atl::shared_ptr<Type> SemanticAnalysis::visit(FunCall &fc) {
  const atl::shared_ptr<FunDecl> funDecl =
      currScope->resolveFunCall(fc.getSignature());
  if (funDecl == nullptr)
    return error("Type Analysis", "Attempted to call undeclared function.",
                 fc.getptr());
  // Resolve the FunDecl/FunDef in TypeAnalysis.
  for (unsigned int idx = 0; idx < fc.funArgs.size(); ++idx)
    fc.funArgs[idx]->accept(*this);

  fc.funDecl = funDecl;
  return fc.funDecl->funType;
}
atl::shared_ptr<Type> SemanticAnalysis::visit(FunDecl &fd) {
  if (currScope->findVarDecl(fd.getIdentifier()))
    return error("Name Analysis",
                 "FunDecl Identifier already in use: " +
                     fd.getIdentifier()->toString(),
                 fd.getptr());

  currScope->insertDecl(fd.getptr());

  for (unsigned int idx = 0; idx < fd.funParams.size(); ++idx)
    fd.funParams[idx]->accept(*this);

  return fd.funType;
}
atl::shared_ptr<Type> SemanticAnalysis::visit(FunDef &fd) {
  if (currScope->findVarDecl(fd.getIdentifier()))
    return error("Name Analysis",
                 "FunDef Identifier already in use: " +
                     fd.getIdentifier()->toString(),
                 fd.getIdentifier());

  currScope->insertDecl(fd.getptr());

  fd.outerScope = currScope;
  currScope = fd.getptr();

  for (unsigned int idx = 0; idx < fd.funParams.size(); ++idx)
    fd.funParams[idx]->accept(*this);
  fd.funBlock->accept(*this);

  currScope = fd.outerScope;

  return fd.funType;
}
atl::shared_ptr<Type> SemanticAnalysis::visit(Identifier &i) {
  // TODO?
  return atl::shared_ptr<BaseType>(new BaseType(PrimitiveType::NULLPTR_T));
}
atl::shared_ptr<Type> SemanticAnalysis::visit(If &i) {
  const atl::shared_ptr<Type> conditionType = i.ifCondition->accept(*this);
  if (conditionType->astClass() != "BaseType")
    return error("Type Analysis", "Type of If condition is invalid.",
                 i.ifCondition);
  atl::shared_ptr<BaseType> conditionBaseType =
      atl::static_pointer_cast<BaseType>(conditionType);
  if (conditionBaseType->primitiveType != PrimitiveType::INT &&
      conditionBaseType->primitiveType != PrimitiveType::BOOL)
    return error("Type Analysis", "Type of If condition is not INT or BOOl.",
                 i.ifCondition);
  i.ifBody->accept(*this);
  if (i.elseBody)
    i.elseBody->accept(*this);

  return atl::shared_ptr<BaseType>(new BaseType(PrimitiveType::NULLPTR_T));
}
atl::shared_ptr<Type> SemanticAnalysis::visit(IntLiteral &il) {
  return atl::shared_ptr<BaseType>(new BaseType(PrimitiveType::INT));
}
atl::shared_ptr<Type> SemanticAnalysis::visit(MemberAccess &ma) {
  const atl::shared_ptr<Type> objectType = ma.object->accept(*this);
  if (objectType->astClass() != "ClassTypeDef")
    return error("Type Analysis",
                 "Attempted to call a member function on a variable that was "
                 "not a object(class instance).",
                 ma.getptr());
  // TODO: Make sure this Class has the member function.
  return ma.fieldVariable->accept(*this);
}
atl::shared_ptr<Type> SemanticAnalysis::visit(MemberCall &mc) {
  const atl::shared_ptr<Type> objectType = mc.object->accept(*this);
  if (objectType->astClass() != "ClassTypeDef")
    return error("Type Analysis",
                 "Attempted to call a member function on a variable that was "
                 "not a object(class instance).",
                 mc.getptr());
  // TODO: Make sure this Class has the member function.
  return mc.funCall->accept(*this);
}
atl::shared_ptr<Type> SemanticAnalysis::visit(Namespace &n) {
  n.outerScope = currScope;
  currScope = n.getptr();

  for (unsigned int i = 0; i < n.namespaceDecls.size(); ++i)
    n.namespaceDecls[i]->accept(*this);

  currScope = n.outerScope;
  return atl::shared_ptr<BaseType>(new BaseType(PrimitiveType::NULLPTR_T));
}
atl::shared_ptr<Type> SemanticAnalysis::visit(Nullptr &n) {

  return atl::shared_ptr<BaseType>(new BaseType(PrimitiveType::NULLPTR_T));
}
atl::shared_ptr<Type> SemanticAnalysis::visit(ParenthExpr &pe) {
  return pe.innerExpr->accept(*this);
}
atl::shared_ptr<Type> SemanticAnalysis::visit(PointerType &pt) {
  return pt.getptr();
}
atl::shared_ptr<Type> SemanticAnalysis::visit(PrefixOp &po) {
  return po.variable->accept(*this);
}
atl::shared_ptr<Type> SemanticAnalysis::visit(Program &p) {
  currScope = atl::make_shared<Block>(Block({}));

  for (unsigned int idx = 0; idx < p.decls.size(); ++idx)
    p.decls[idx]->accept(*this);

  p.globalScope = currScope;
  return atl::shared_ptr<BaseType>(new BaseType(PrimitiveType::NULLPTR_T));
}
atl::shared_ptr<Type> SemanticAnalysis::visit(ReferenceType &rt) {
  rt.referencedType->accept(*this);
  return rt.getptr();
}
atl::shared_ptr<Type> SemanticAnalysis::visit(Return &r) {
  if (r.returnExpr)
    return r.returnExpr->accept(*this);

  return atl::shared_ptr<BaseType>(new BaseType(PrimitiveType::VOID));
}
atl::shared_ptr<Type> SemanticAnalysis::visit(SizeOf &so) {
  return atl::shared_ptr<BaseType>(new BaseType(PrimitiveType::INT));
}
atl::shared_ptr<Type> SemanticAnalysis::visit(StringLiteral &sl) {
  return atl::shared_ptr<PointerType>(new PointerType(
      atl::shared_ptr<BaseType>(new BaseType(PrimitiveType::CHAR))));
}
atl::shared_ptr<Type> SemanticAnalysis::visit(StructType &st) {
  // TODO: Check StructTypeDecl exists.
  return st.getptr();
}
atl::shared_ptr<Type> SemanticAnalysis::visit(StructTypeDecl &std) {
  // TODO
  // if (currScope->duplicateDeclarationLocal(std.getptr()))
  //   return error("Attempted to declare a Struct with an identifier that is "
  //                "already in use: " +
  //                    std.getIdentifier()->toString(),
  //                std.getptr());

  // currScope->insertDecl(std.getptr());

  // std.outerScope = currScope;
  // currScope = std.getptr();

  // /* Check that the fields in this struct are unique */
  // for (unsigned int idx = 0; idx < std.varDecls.size(); ++idx) {
  //   if (currScope->duplicateDeclarationLocal(std.varDecls[idx]))
  //     return error("Struct " + std.getIdentifier()->toString() +
  //                      " contained multiple fields with the same identifier:
  //                      " + std.varDecls[idx]->getIdentifier()->toString(),
  //                  atl::static_pointer_cast<Decl>(std.varDecls[idx]));
  //   std.varDecls[idx]->accept(*this);
  // }

  // currScope = std.outerScope;
  return atl::shared_ptr<BaseType>(new BaseType(PrimitiveType::NULLPTR_T));
}
atl::shared_ptr<Type> SemanticAnalysis::visit(TertiaryExpr &t) {
  const atl::shared_ptr<Type> conditionType =
      t.tertiaryCondition->accept(*this);
  if (conditionType->astClass() != "BaseType")
    return error("Type Analysis", "Type of While condition is invalid.",
                 t.tertiaryCondition);
  atl::shared_ptr<BaseType> conditionBaseType =
      atl::static_pointer_cast<BaseType>(conditionType);
  if (conditionBaseType->primitiveType != PrimitiveType::INT &&
      conditionBaseType->primitiveType != PrimitiveType::BOOL)
    return error("Type Analysis", "Type of While condition is not INT or BOOl.",
                 t.tertiaryCondition);

  const atl::shared_ptr<Type> ifBodyType = t.tertiaryIfBody->accept(*this);
  const atl::shared_ptr<Type> elseBodyType = t.tertiaryElseBody->accept(*this);
  if (*ifBodyType != *elseBodyType)
    return error("Type Analysis",
                 "Types of Tertiary conditions are not the same.", t.getptr());

  return ifBodyType;
}
atl::shared_ptr<Type> SemanticAnalysis::visit(Throw &t) {
  // TODO
  return atl::shared_ptr<BaseType>(new BaseType(PrimitiveType::NULLPTR_T));
}
atl::shared_ptr<Type> SemanticAnalysis::visit(TypeCast &tc) {
  tc.expr->accept(*this);
  return tc.type;
}
atl::shared_ptr<Type> SemanticAnalysis::visit(TypeDefDecl &tdd) {
  // TODO:
  return atl::shared_ptr<BaseType>(new BaseType(PrimitiveType::NULLPTR_T));
}
atl::shared_ptr<Type> SemanticAnalysis::visit(ValueAt &va) {
  const atl::shared_ptr<Type> exprType = va.derefExpr->accept(*this);
  if (exprType->astClass() != "PointerType")
    return error("Type Analysis",
                 "Attempted to dereference variable that wasn't a pointer. ",
                 va.derefExpr);
  return atl::static_pointer_cast<PointerType>(exprType)->pointedType;
}
atl::shared_ptr<Type> SemanticAnalysis::visit(VarDecl &vd) {
  if (vd.type->astClass() == "ClassType") {
    const atl::shared_ptr<ClassTypeDef> vdTypeDecl =
        currScope->findClassDef(vd.getIdentifier());

    if (vdTypeDecl == nullptr)
      return error("Type Analysis",
                   "Attempted to declare variable with undefined class type.",
                   atl::static_pointer_cast<Decl>(vd.getptr()));
  }
  if (currScope->findVarDecl(vd.getIdentifier()))
    return error("Name Analysis",
                 "Attempted to declare a Variable with an identifier that is "
                 "already in use: " +
                     vd.getIdentifier()->toString(),
                 atl::static_pointer_cast<Decl>(vd.getptr()));
  currScope->insertDecl(vd.getptr());

  return atl::shared_ptr<BaseType>(new BaseType(PrimitiveType::NULLPTR_T));
}
atl::shared_ptr<Type> SemanticAnalysis::visit(VarDef &vd) {
  if (vd.type->astClass() == "ClassType") {
    const atl::shared_ptr<ClassTypeDef> vdTypeDecl =
        currScope->findClassDef(vd.getIdentifier());

    if (vdTypeDecl == nullptr)
      return error("Type Analysis",
                   "Attempted to define variable with undefined class type.",
                   atl::static_pointer_cast<Decl>(vd.getptr()));
  }
  if (currScope->findVarDecl(vd.getIdentifier()))
    return error("Name Analysis",
                 "Attempted to define a Variable with an identifier that is "
                 "already in use: " +
                     vd.getIdentifier()->toString(),
                 atl::static_pointer_cast<Decl>(vd.getptr()));
  currScope->insertDecl(vd.getptr());

  return atl::shared_ptr<BaseType>(new BaseType(PrimitiveType::NULLPTR_T));
}
atl::shared_ptr<Type> SemanticAnalysis::visit(VarExpr &ve) {
  const atl::shared_ptr<VarDecl> varDecl =
      currScope->findVarDecl(ve.varIdentifier);
  if (varDecl == nullptr)
    return error("Name Analysis",
                 "Attempted to reference undeclared variable: " +
                     ve.varIdentifier->toString(),
                 ve.getptr());
  ve.varDecl = varDecl;
  return varDecl->type;
}
atl::shared_ptr<Type> SemanticAnalysis::visit(While &w) {
  atl::shared_ptr<Type> conditionType = w.condition->accept(*this);
  if (conditionType->astClass() != "BaseType")
    return error("Type Analysis", "Type of While condition is invalid.",
                 w.condition);
  atl::shared_ptr<BaseType> conditionBaseType =
      atl::static_pointer_cast<BaseType>(conditionType);
  if (conditionBaseType->primitiveType != PrimitiveType::INT &&
      conditionBaseType->primitiveType != PrimitiveType::BOOL)
    return error("Type Analysis", "Type of While condition is not INT or BOOl.",
                 w.condition);
  w.body->accept(*this);
  return atl::shared_ptr<BaseType>(new BaseType(PrimitiveType::NULLPTR_T));
}