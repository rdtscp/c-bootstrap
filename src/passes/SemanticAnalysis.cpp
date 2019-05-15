#include "atl/include/set.h"

#include "Error.h"
#include "passes/SemanticAnalysis.h"

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
  const unsigned int num_errors = errors.size();
  if (num_errors > 0)
    printf("Semantic Analysis Errors:\n");
  for (unsigned int idx = 0; idx < num_errors; ++idx)
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
atl::shared_ptr<Type> SemanticAnalysis::visit(ArrayType &at) {
  return at.getptr();
}
atl::shared_ptr<Type> SemanticAnalysis::visit(Assign &as) {
  const atl::shared_ptr<Type> lhsType = as.lhs->accept(*this);
  const atl::shared_ptr<Type> rhsType = as.rhs->accept(*this);
  if (*lhsType != *rhsType)
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
    return error("Type Analysis", "Binary operation has mismatched types.",
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

  for (unsigned int idx = 0; idx < b.stmts.size(); ++idx) {
    b.stmts[idx]->accept(*this);
    ++b.stmtsChecked;
  }

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
      currScope->findClassDef(ct.identifier);

  if (ctd == nullptr)
    return error("Name Analysis",
                 "Attempted to use a Class that was not declared: " +
                     ct.identifier->toString(),
                 ct.getptr());

  ct.typeDefinition = ctd;
  return ct.getptr();
}
atl::shared_ptr<Type> SemanticAnalysis::visit(ClassTypeDecl &ctd) {
  if (currScope->findClassDef(ctd.getIdentifier(), ctd.getptr()))
    return error("Name Analysis",
                 "Attempted to declare a Class with an identifier that is "
                 "already in use: " +
                     ctd.getIdentifier()->toString(),
                 ctd.getptr());

  return ctd.classType;
}
atl::shared_ptr<Type> SemanticAnalysis::visit(ClassTypeDef &ctd) {
  if (currScope->findClassDef(ctd.getIdentifier(), ctd.getptr()))
    return error("Name Analysis",
                 "Attempted to declare a Class with an identifier that is "
                 "already in use: " +
                     ctd.getIdentifier()->toString(),
                 ctd.getptr());

  ctd.outerScope = currScope;
  currScope = ctd.getptr();

  for (unsigned int idx = 0; idx < ctd.classDecls.size(); ++idx)
    ctd.classDecls[idx]->accept(*this);

  currScope = ctd.outerScope;

  return ctd.classType;
}
atl::shared_ptr<Type> SemanticAnalysis::visit(ConstructorCall &cc) {
  const atl::shared_ptr<ClassTypeDef> ctorClassTypeDef =
      currScope->findClassDef(cc.constructorIdentifier);

  atl::vector<atl::shared_ptr<Type>> constructorCallArgTypes;
  constructorCallArgTypes.push_back(atl::shared_ptr<PointerType>(
      new PointerType(ctorClassTypeDef->classType)));
  for (unsigned int idx = 0; idx < cc.constructorArgs.size(); ++idx)
    constructorCallArgTypes.push_back(cc.constructorArgs[idx]->accept(*this));

  const FunSignature ctorCallSignature(nullptr, cc.constructorIdentifier,
                                       constructorCallArgTypes);
  const atl::shared_ptr<ConstructorDecl> ctorDecl =
      ctorClassTypeDef->findConstructorDecl(ctorCallSignature);
  if (ctorDecl == nullptr)
    return error("Type Analysis", "Attempted to call undeclared constructor.",
                 cc.getptr());

  cc.constructorDecl = ctorDecl;
  return cc.constructorDecl->classType;
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
  atl::vector<atl::shared_ptr<Type>> funCallArgTypes;
  for (unsigned int idx = 0; idx < fc.funArgs.size(); ++idx)
    funCallArgTypes.push_back(fc.funArgs[idx]->accept(*this));

  const FunSignature funCallSignature(nullptr, fc.funIdentifier,
                                      funCallArgTypes);
  const atl::shared_ptr<FunDecl> funDecl =
      currScope->findFunDecl(funCallSignature);
  if (funDecl == nullptr)
    return error("Type Analysis", "Attempted to call undeclared function.",
                 fc.getptr());

  fc.funDecl = funDecl;
  return fc.funDecl->funType;
}
atl::shared_ptr<Type> SemanticAnalysis::visit(FunDecl &fd) {
  if (currScope->findVarDecl(fd.getIdentifier(), fd.getptr()))
    return error("Name Analysis",
                 "FunDecl Identifier already in use: " +
                     fd.getIdentifier()->toString(),
                 fd.getptr());

  for (unsigned int idx = 0; idx < fd.funParams.size(); ++idx)
    fd.funParams[idx]->accept(*this);

  return fd.funType->accept(*this);
}
atl::shared_ptr<Type> SemanticAnalysis::visit(FunDef &fd) {
  if (currScope->findVarDecl(fd.getIdentifier(), fd.getptr()))
    return error("Name Analysis",
                 "FunDef Identifier already in use: " +
                     fd.getIdentifier()->toString(),
                 fd.getIdentifier());

  fd.outerScope = currScope;
  currScope = fd.getptr();

  for (unsigned int idx = 0; idx < fd.funParams.size(); ++idx)
    fd.funParams[idx]->accept(*this);
  fd.funBlock->accept(*this);

  return fd.funType->accept(*this);
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
  objType = collapseReferenceTypes(objType);

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
  objType = collapseReferenceTypes(objType);

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
  atl::vector<atl::shared_ptr<Type>> funCallArgTypes;
  funCallArgTypes.push_back(atl::shared_ptr<PointerType>(
      new PointerType(objClassTypeDef->classType)));
  for (unsigned int idx = 0; idx < mc.funCall->funArgs.size(); ++idx)
    funCallArgTypes.push_back(mc.funCall->funArgs[idx]->accept(*this));

  const FunSignature funCallSignature(nullptr, mc.funCall->funIdentifier,
                                      funCallArgTypes);
  const atl::shared_ptr<FunDecl> memberFunDecl =
      objClassTypeDef->findFunDeclLocal(funCallSignature);
  if (memberFunDecl == nullptr)
    return error("Type Error",
                 "Attempted to call a member function that does "
                 "not exist in the class definition.",
                 mc.funCall);

  return memberFunDecl->funType;
}
atl::shared_ptr<Type> SemanticAnalysis::visit(Namespace &n) {
  n.outerScope = currScope;
  currScope = n.getptr();

  for (unsigned int i = 0; i < n.namespaceDecls.size(); ++i) {
    n.namespaceDecls[i]->accept(*this);
    ++n.namespaceDeclsChecked;
  }

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
  currScope = p.getptr();

  for (unsigned int idx = 0; idx < p.decls.size(); ++idx) {
    p.decls[idx]->accept(*this);
    ++p.declsChecked;
  }

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
atl::shared_ptr<Type> SemanticAnalysis::visit(SubscriptOp &so) {
  atl::shared_ptr<Type> objType = so.variable->accept(*this);
  objType = collapseReferenceTypes(objType);
  if (objType->astClass() == "ArrayType" ||
      objType->astClass() == "PointerType") {
    // TODO: Initialise this FunDef with an implementation.
    const atl::shared_ptr<FunDef> arrayPointerSubscriptOpDef;
    so.operatorDecl = arrayPointerSubscriptOpDef;
    return atl::static_pointer_cast<PointerType>(objType)->pointedType;
  } else if (objType->astClass() == "ClassType") {
    const atl::shared_ptr<ClassType> objClassType =
        atl::static_pointer_cast<ClassType>(objType);
    const atl::shared_ptr<ClassTypeDef> objClassTypeDef =
        objClassType->typeDefinition;

    const atl::shared_ptr<Type> indexType = so.index->accept(*this);

    // Create FunSignature for SubscriptOp.
    atl::vector<atl::shared_ptr<Type>> opArgs;
    opArgs.push_back(
        atl::shared_ptr<PointerType>(new PointerType(objClassType)));
    opArgs.push_back(so.index->accept(*this));
    const atl::shared_ptr<Identifier> opIdentifier(
        new Identifier("operator[]", objClassType->identifier));
    const FunSignature opSignature(nullptr, opIdentifier, opArgs);

    // TODO: Consider if this should search all the scope, or just the scope for
    // the ClassTypeDef already resolved above. The former would allow us to use
    // out of class definitions.
    const atl::shared_ptr<FunDecl> objSubscriptOpDecl =
        currScope->findFunDecl(opSignature);
    if (objSubscriptOpDecl == nullptr) {
      return error("Type Error",
                   "No definiton for subscript operator[] for type: " +
                       objClassType->identifier->toString(),
                   so.variable);
    }

    so.operatorDecl = objSubscriptOpDecl;
    return objSubscriptOpDecl->funType;
  } else {
    return error("Type Error",
                 "Cannot perform subscript operator[] on type: " +
                     objType->astClass(),
                 so.variable);
  }
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
    const atl::shared_ptr<ClassType> vdClassType = vd.type->accept(*this);
    if (vdClassType->typeDefinition == nullptr)
      return error("Type Analysis",
                   "Attempted to define variable with undefined class type.",
                   atl::static_pointer_cast<Decl>(vd.getptr()));
  }
  if (currScope->findVarDecl(vd.getIdentifier(), vd.getptr()))
    return error("Name Analysis",
                 "Attempted to declare a Variable with an identifier that is "
                 "already in use: " +
                     vd.getIdentifier()->toString(),
                 atl::static_pointer_cast<Decl>(vd.getptr()));

  return atl::shared_ptr<BaseType>(new BaseType(PrimitiveType::NULLPTR_T));
}
atl::shared_ptr<Type> SemanticAnalysis::visit(VarDef &vd) {
  if (vd.type->astClass() == "ClassType") {
    const atl::shared_ptr<ClassType> vdClassType = vd.type->accept(*this);
    if (vdClassType->typeDefinition == nullptr)
      return error("Type Analysis",
                   "Attempted to define variable with undefined class type.",
                   atl::static_pointer_cast<Decl>(vd.getptr()));
  }
  if (currScope->findVarDecl(vd.getIdentifier(), vd.getptr()))
    return error("Name Analysis",
                 "Attempted to define a Variable with an identifier that is "
                 "already in use: " +
                     vd.getIdentifier()->toString(),
                 atl::static_pointer_cast<Decl>(vd.getptr()));

  // Visit the value initialised.
  const atl::shared_ptr<Type> valueType = vd.varValue->accept(*this);
  if (*valueType != *vd.type)
    return error("Type Analysis", "VarDef has mismatched types.",
                 atl::static_pointer_cast<Decl>(vd.getptr()));

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
  if (conditionBaseType->primitiveType != PrimitiveType::BOOL &&
      conditionBaseType->primitiveType != PrimitiveType::INT &&
      conditionBaseType->primitiveType != PrimitiveType::UINT)
    return error("Type Analysis",
                 "Type of While condition is not BOOL, INT or UINT.",
                 w.condition);
  w.body->accept(*this);
  return atl::shared_ptr<BaseType>(new BaseType(PrimitiveType::NULLPTR_T));
}
atl::shared_ptr<Type>
SemanticAnalysis::collapseReferenceTypes(atl::shared_ptr<Type> type) {
  if (type->astClass() == "ReferenceType") {
    type = atl::static_pointer_cast<ReferenceType>(type)->referencedType;
    if (type->astClass() == "ReferenceType")
      type = atl::static_pointer_cast<ReferenceType>(type)->referencedType;
  }
  return type;
}
