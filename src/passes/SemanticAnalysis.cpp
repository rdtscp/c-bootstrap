#include "atl/include/set.h"

#include "Error.h"
#include "passes/SemanticAnalysis.h"

using namespace ACC;

SemanticAnalysis::SemanticAnalysis(atl::shared_ptr<Program> progAST)
    : progAST(progAST), inClassTypeDef(false) {}

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
  // Make sure its the address of a named variable.
  if (ao.addressOfExpr->astClass() != "VarExpr") {
    return error("Type Error", "Cannot take the address of non LVALUE",
                 ao.getptr());
  }
  // Make sure the named variable is not a T&&.
  const atl::shared_ptr<Type> exprType = ao.addressOfExpr->accept(*this);
  if (exprType->astClass() == "ReferenceType") {
    const atl::shared_ptr<ReferenceType> refExprType =
        atl::static_pointer_cast<ReferenceType>(exprType);
    if (refExprType->referencedType->astClass() == "ReferenceType")
      return error("Type Error", "Cannot take the address of non LVALUE.",
                   ao.getptr());
  }
  return atl::make_shared<PointerType>(PointerType(exprType));
}
atl::shared_ptr<Type> SemanticAnalysis::visit(Allocation &a) {
  if (a.varType != nullptr)
    return a.varType;

  return atl::shared_ptr<PointerType>(
      new PointerType(a.varConstructorCall->accept(*this)));
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

  return atl::static_pointer_cast<ArrayType>(arrayExprType)->pointedType;
}
atl::shared_ptr<Type> SemanticAnalysis::visit(ArrayType &at) {
  return at.getptr();
}
atl::shared_ptr<Type> SemanticAnalysis::visit(Assign &as) {
  const atl::shared_ptr<Type> lhsType = as.lhs->accept(*this);
  const atl::shared_ptr<Type> rhsType = as.rhs->accept(*this);
  if (*lhsType != *rhsType)
    return error("Type Analysis",
                 "Assignation has mismatched types. (" +
                     lhsType->getSignature() + " = " + rhsType->getSignature() +
                     ")",
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
    return error("Type Analysis",
                 "Binary operation has mismatched types. (" +
                     lhsType->getSignature() + ", " + rhsType->getSignature() +
                     ")",
                 bo.getptr());

  switch (bo.operation) {
  case Op::ADD:
    return lhsType;
  case Op::SUB:
    return lhsType;
  case Op::MUL:
    return lhsType;
  case Op::DIV:
    return lhsType;
  case Op::MOD:
    return atl::shared_ptr<BaseType>(new BaseType(PrimitiveType::UINT));
  case Op::GT:
    return atl::shared_ptr<BaseType>(new BaseType(PrimitiveType::BOOL));
  case Op::LT:
    return atl::shared_ptr<BaseType>(new BaseType(PrimitiveType::BOOL));
  case Op::GE:
    return atl::shared_ptr<BaseType>(new BaseType(PrimitiveType::BOOL));
  case Op::LE:
    return atl::shared_ptr<BaseType>(new BaseType(PrimitiveType::BOOL));
  case Op::NE:
    return atl::shared_ptr<BaseType>(new BaseType(PrimitiveType::BOOL));
  case Op::EQ:
    return atl::shared_ptr<BaseType>(new BaseType(PrimitiveType::BOOL));
  case Op::OR:
    return atl::shared_ptr<BaseType>(new BaseType(PrimitiveType::BOOL));
  case Op::AND:
    return atl::shared_ptr<BaseType>(new BaseType(PrimitiveType::BOOL));
  case Op::ASSIGNADD:
    return lhsType;
  }
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

  inClassTypeDef = true;
  ctd.outerScope = currScope;
  currScope = ctd.getptr();

  for (unsigned int idx = 0; idx < ctd.classDecls.size(); ++idx)
    currScope->insertDecl(ctd.classDecls[idx]);

  for (unsigned int idx = 0; idx < ctd.classDecls.size(); ++idx)
    ctd.classDecls[idx]->accept(*this);

  currScope = ctd.outerScope;
  inClassTypeDef = true;

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

  const bool inClassTypeDef_temp = inClassTypeDef;
  inClassTypeDef = false;

  for (unsigned int idx = 0; idx < cd.constructorParams.size(); ++idx)
    cd.constructorParams[idx]->accept(*this);
  cd.constructorBlock->accept(*this);

  inClassTypeDef = inClassTypeDef_temp;

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
  const bool inClassTypeDef_temp = inClassTypeDef;
  inClassTypeDef = false;

  dd.destructorBlock->accept(*this);

  inClassTypeDef = inClassTypeDef_temp;

  return atl::shared_ptr<BaseType>(new BaseType(PrimitiveType::NULLPTR_T));
}
atl::shared_ptr<Type> SemanticAnalysis::visit(DoWhile &dw) {
  const atl::shared_ptr<Type> conditionType = dw.condition->accept(*this);
  if (conditionType->astClass() != "BaseType")
    return error("Type Analysis", "Type of If condition is invalid.",
                 dw.condition);
  atl::shared_ptr<BaseType> conditionBaseType =
      atl::static_pointer_cast<BaseType>(conditionType);
  if (conditionBaseType->primitiveType != PrimitiveType::BOOL &&
      conditionBaseType->primitiveType != PrimitiveType::INT &&
      conditionBaseType->primitiveType != PrimitiveType::UINT)
    return error("Type Analysis",
                 "Type of If condition is not BOOL, INT or UINT.",
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
  if (conditionBaseType->primitiveType != PrimitiveType::BOOL &&
      conditionBaseType->primitiveType != PrimitiveType::INT &&
      conditionBaseType->primitiveType != PrimitiveType::UINT)
    return error("Type Analysis",
                 "Type of For condition is not BOOL, INT or UINT.",
                 f.condition);
  f.endBodyExpr->accept(*this);
  f.body->accept(*this);

  currScope = f.outerScope;
  return atl::shared_ptr<BaseType>(new BaseType(PrimitiveType::NULLPTR_T));
}
atl::shared_ptr<Type> SemanticAnalysis::visit(FunCall &fc) {
  // Visit all parameters first.
  for (unsigned int idx = 0; idx < fc.funArgs.size(); ++idx)
    fc.funArgs[idx]->accept(*this);

  const atl::shared_ptr<FunDecl> funDecl =
      currScope->resolveFunCall(fc.getSignature());
  if (funDecl == nullptr)
    return error("Type Analysis", "Attempted to call undeclared function.",
                 fc.getptr());

  fc.funDecl = funDecl;
  return fc.funDecl->funType;
}
atl::shared_ptr<Type> SemanticAnalysis::visit(FunDecl &fd) {
  if (!inClassTypeDef && currScope->findVarDecl(fd.getIdentifier()))
    return error("Name Analysis",
                 "FunDecl Identifier already in use: " +
                     fd.getIdentifier()->toString(),
                 fd.getptr());

  if (!inClassTypeDef)
    currScope->insertDecl(fd.getptr());

  for (unsigned int idx = 0; idx < fd.funParams.size(); ++idx)
    fd.funParams[idx]->accept(*this);

  return fd.funType;
}
atl::shared_ptr<Type> SemanticAnalysis::visit(FunDef &fd) {
  if (!inClassTypeDef && currScope->findVarDecl(fd.getIdentifier()))
    return error("Name Analysis",
                 "FunDef Identifier already in use: " +
                     fd.getIdentifier()->toString(),
                 fd.getIdentifier());

  if (!inClassTypeDef)
    currScope->insertDecl(fd.getptr());

  fd.outerScope = currScope;
  currScope = fd.getptr();

  const bool inClassTypeDef_temp = inClassTypeDef;
  inClassTypeDef = false;

  for (unsigned int idx = 0; idx < fd.funParams.size(); ++idx)
    fd.funParams[idx]->accept(*this);
  fd.funBlock->accept(*this);

  inClassTypeDef = inClassTypeDef_temp;
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
  if (conditionBaseType->primitiveType != PrimitiveType::BOOL &&
      conditionBaseType->primitiveType != PrimitiveType::INT &&
      conditionBaseType->primitiveType != PrimitiveType::UINT)
    return error("Type Analysis",
                 "Type of If condition is not BOOL, INT or UINT.",
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
  atl::shared_ptr<Type> objType = ma.object->accept(*this);

  atl::shared_ptr<ClassType> objClassType;
  if (objType->astClass() == "ClassType") {
    if (ma.accessType != SourceToken::Class::DOT)
      return error("Type Analysis",
                   "Attempted to access member variable of class type without "
                   "using `.` operator.",
                   ma.object);
    objClassType = atl::static_pointer_cast<ClassType>(objType);
  } else if (objType->astClass() == "PointerType") {
    if (ma.accessType != SourceToken::Class::PTRDOT)
      return error("Type Analysis",
                   "Attempted to access member variable of class type without "
                   "using `.` operator.",
                   ma.object);
    objType = atl::static_pointer_cast<PointerType>(objType)->pointedType;
    // Get ClassType
    if (objType->astClass() != "ClassType")
      return error("Type Analysis",
                   "Attempted to access a member variable on a variable that "
                   "was not an object.",
                   ma.object);
    objClassType = atl::static_pointer_cast<ClassType>(objType);
  } else if (objType->astClass() == "ReferenceType") {
    if (ma.accessType != SourceToken::Class::DOT)
      return error("Type Analysis",
                   "Attempted to access member variable of class type without "
                   "using `.` operator.",
                   ma.object);
    objType = atl::static_pointer_cast<ReferenceType>(objType)->referencedType;
    // Handle T&&
    if (objType->astClass() == "ReferenceType")
      objType =
          atl::static_pointer_cast<ReferenceType>(objType)->referencedType;
    // Get ClassType
    if (objType->astClass() != "ClassType")
      return error("Type Analysis",
                   "Attempted to access a member variable on a variable that "
                   "was not an object.",
                   ma.object);
    objClassType = atl::static_pointer_cast<ClassType>(objType);
  } else {
    return error("Type Analysis",
                 "Attempted to access a member variable on a variable that "
                 "was not an object.",
                 ma.object);
  }

  const atl::shared_ptr<ClassTypeDef> objClassTypeDef =
      objClassType->typeDefinition;
  // Check this ClassType is Defined.
  if (objClassTypeDef == nullptr)
    return error("Type Error",
                 "Attempted to access member variable of class type that has "
                 "no definition.",
                 ma.object);

  const atl::shared_ptr<Scope> outerScope = currScope;
  currScope = objClassTypeDef;

  /* Resolve the underlying type */
  const atl::shared_ptr<Type> memberType = ma.fieldVariable->accept(*this);

  currScope = outerScope;

  return memberType;
}
atl::shared_ptr<Type> SemanticAnalysis::visit(MemberCall &mc) {
  atl::shared_ptr<Type> objType = mc.object->accept(*this);
  // const atl::shared_ptr<Type> memberCallType = mc.funCall->accept(*this);

  atl::shared_ptr<ClassType> objClassType;
  if (objType->astClass() == "ClassType") {
    if (mc.accessType != SourceToken::Class::DOT)
      return error("Type Analysis",
                   "Attempted to call member function of class type without "
                   "using `.` operator.",
                   mc.object);
    objClassType = atl::static_pointer_cast<ClassType>(objType);
  } else if (objType->astClass() == "PointerType") {
    if (mc.accessType != SourceToken::Class::PTRDOT)
      return error("Type Analysis",
                   "Attempted to access member variable of class type without "
                   "using `.` operator.",
                   mc.object);
    objType = atl::static_pointer_cast<PointerType>(objType)->pointedType;
    // Get ClassType
    if (objType->astClass() != "ClassType")
      return error("Type Analysis",
                   "Attempted to call a member function on a variable that "
                   "was not an object.",
                   mc.object);
    objClassType = atl::static_pointer_cast<ClassType>(objType);
  } else if (objType->astClass() == "ReferenceType") {
    if (mc.accessType != SourceToken::Class::DOT)
      return error("Type Analysis",
                   "Attempted to call member function of class type without "
                   "using `.` operator.",
                   mc.object);
    objType = atl::static_pointer_cast<ReferenceType>(objType)->referencedType;
    // Handle T&&
    if (objType->astClass() == "ReferenceType")
      objType =
          atl::static_pointer_cast<ReferenceType>(objType)->referencedType;
    // Get ClassType
    if (objType->astClass() != "ClassType")
      return error("Type Analysis",
                   "Attempted to call a member function on a variable that was "
                   "not an object.",
                   mc.object);
    objClassType = atl::static_pointer_cast<ClassType>(objType);
  } else {
    return error("Type Analysis",
                 "Attempted to access a member variable on a variable that "
                 "was not an object.",
                 mc.object);
  }

  const atl::shared_ptr<ClassTypeDef> objClassTypeDef =
      objClassType->typeDefinition;
  // Check this ClassType is Defined.
  if (objClassTypeDef == nullptr)
    return error("Type Error",
                 "Attempted to call member function of class type that has "
                 "no definition.",
                 mc.object);

  /* Now Manually Visit the Member Call */
  // Visit all parameters first.
  for (unsigned int idx = 0; idx < mc.funCall->funArgs.size(); ++idx)
    mc.funCall->funArgs[idx]->accept(*this);

  // Check this ClassTypeDef contains the member.
  const atl::shared_ptr<FunDecl> memberFunDecl =
      objClassTypeDef->findFunDeclLocal(mc.funCall->getSignature());
  if (memberFunDecl == nullptr)
    return error("Type Error",
                 "Attempted to call a member function that does "
                 "not exist in the class definition.",
                 mc.funCall);

  // return mc.funCall->accept(*this);
  return memberFunDecl->funType;
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
atl::shared_ptr<Type> SemanticAnalysis::visit(TertiaryExpr &t) {
  const atl::shared_ptr<Type> conditionType =
      t.tertiaryCondition->accept(*this);
  if (conditionType->astClass() != "BaseType")
    return error("Type Analysis", "Type of While condition is invalid.",
                 t.tertiaryCondition);
  atl::shared_ptr<BaseType> conditionBaseType =
      atl::static_pointer_cast<BaseType>(conditionType);
  if (conditionBaseType->primitiveType != PrimitiveType::BOOL &&
      conditionBaseType->primitiveType != PrimitiveType::INT &&
      conditionBaseType->primitiveType != PrimitiveType::UINT)
    return error("Type Analysis",
                 "Type of While condition is not BOOL, INT or UINT.",
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
    const atl::shared_ptr<ClassType> vdType =
        atl::static_pointer_cast<ClassType>(vd.type);
    const atl::shared_ptr<ClassTypeDef> vdTypeDecl =
        currScope->findClassDef(vdType->identifier);

    if (vdTypeDecl == nullptr)
      return error("Type Analysis",
                   "Attempted to declare variable with undefined class type.",
                   atl::static_pointer_cast<Decl>(vd.getptr()));
  }
  if (!inClassTypeDef && currScope->findVarDecl(vd.getIdentifier()))
    return error("Name Analysis",
                 "Attempted to declare a Variable with an identifier that is "
                 "already in use: " +
                     vd.getIdentifier()->toString(),
                 atl::static_pointer_cast<Decl>(vd.getptr()));

  if (!inClassTypeDef)
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

  if (!inClassTypeDef)
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
  return varDecl->type->accept(*this);
}
atl::shared_ptr<Type> SemanticAnalysis::visit(While &w) {
  atl::shared_ptr<Type> conditionType = w.condition->accept(*this);
  if (conditionType->astClass() != "BaseType")
    return error("Type Analysis", "Type of While condition is invalid.",
                 w.condition);
  atl::shared_ptr<BaseType> conditionBaseType =
      atl::static_pointer_cast<BaseType>(conditionType);
  if (conditionBaseType->primitiveType != PrimitiveType::BOOL &&
      conditionBaseType->primitiveType != PrimitiveType::INT &&
      conditionBaseType->primitiveType != PrimitiveType::UINT)
    return error("Type Analysis",
                 "Type of While condition is not BOOL, INT or UINT.",
                 w.condition);
  w.body->accept(*this);
  return atl::shared_ptr<BaseType>(new BaseType(PrimitiveType::NULLPTR_T));
}