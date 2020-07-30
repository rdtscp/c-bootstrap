#include "atl/include/set.h"

#include "Error.h"
#include "passes/SemanticAnalysis.h"

using namespace ACC;

SemanticAnalysis::SemanticAnalysis(atl::shared_ptr<Program> progAST)
    : progAST(progAST) {}

SemanticAnalysis::~SemanticAnalysis() {
  if (errorCount != 0) {
    printErrors();
  }
}

atl::shared_ptr<Type>
SemanticAnalysis::error(const atl::string &errorType, const atl::string &error,
                        const atl::shared_ptr<ASTNode> &node) {
  errorCount++;
  errors.push_back(errorType + " Error at: " + node->position.toString() +
                   "\n\t" + error);
  if (errorCount == 9) {
    printErrors();
    throw Error("9 Semantic Errors: Exiting Prematurely");
  }
  return noType();
}

void SemanticAnalysis::printErrors() {
  const unsigned int num_errors = errors.size();
  if (num_errors > 0)
    printf("Semantic Analysis Errors:\n");
  for (unsigned int idx = 0; idx < num_errors; ++idx)
    printf("\t%s\n", errors[idx].c_str());
}

/* TODO: Link up namespaces in such a way that looking at any given FunDecl or
 * FunDef will have its Identifier be global and correct. */

void SemanticAnalysis::run() { visit(*progAST); }

/* ---- Visit AST ---- */

atl::shared_ptr<Type> SemanticAnalysis::visit(AddressOf &ao) {
  // Make sure the named variable is not a T&&.
  const atl::shared_ptr<Type> exprType = ao.addressOfExpr->accept(*this);
  if (exprType->astClass() == "ReferenceType") {
    const atl::shared_ptr<ReferenceType> refExprType =
        atl::static_pointer_cast<ReferenceType>(exprType);
    if (refExprType->referencedType->astClass() == "ReferenceType")
      return error("Type Error", "Cannot take the address of non LVALUE.",
                   ao.getptr());
  }
  return atl::shared_ptr<PointerType>(new PointerType(exprType));
}
atl::shared_ptr<Type> SemanticAnalysis::visit(Allocation &a) {
  if (a.varType != nullptr)
    return a.varType->accept(*this);

  return atl::shared_ptr<PointerType>(
      new PointerType(a.varConstructorCall->accept(*this)));
}
atl::shared_ptr<Type> SemanticAnalysis::visit(ArrayType &at) {
  at.size->accept(*this);
  return at.getptr();
}
atl::shared_ptr<Type> SemanticAnalysis::visit(Assign &as) {
  const atl::shared_ptr<Type> lhsType = as.lhs->accept(*this);
  if (lhsType == nullptr)
    return error("Type Analysis", "Assignation LHS has undefined type.",
                 as.getptr());
  const atl::shared_ptr<Type> rhsType = as.rhs->accept(*this);
  if (lhsType == nullptr) {
    return error("Type Analysis", "Assignation LHS has undefined type.",
                 as.getptr());
  }
  if (rhsType == nullptr) {
    return error("Type Analysis", "Assignation RHS has undefined type.",
                 as.getptr());
  }
  if (!lhsType->equivalentTo(*rhsType) && *lhsType != *rhsType) {
    return error("Type Analysis", "Assignation has mismatched types.",
                 as.getptr());
  }
  return noType();
}
atl::shared_ptr<Type> SemanticAnalysis::visit(BaseType &bt) {
  return bt.getptr();
}
atl::shared_ptr<Type> SemanticAnalysis::visit(BinOp &bo) {
  const atl::shared_ptr<Type> lhsType =
      ReferenceType::collapseReferenceTypes(bo.lhs->accept(*this));
  const atl::shared_ptr<Type> rhsType =
      ReferenceType::collapseReferenceTypes(bo.rhs->accept(*this));

  if (lhsType->astClass() == "ClassType") {
    const atl::shared_ptr<ClassType> lhsClassType =
        atl::static_pointer_cast<ClassType>(lhsType);

    /*  Precedence for operator overloading:
     *    1) Freestanding Functions
     */
    const atl::shared_ptr<ClassTypeDef> lhsClassTypeDef =
        lhsClassType->typeDefinition.lock();
    if (lhsClassTypeDef == nullptr) {
      return error("Type Analysis",
                   "No type definition for LHS of binary operation: " +
                       lhsClassType->identifier->toString(),
                   bo.getptr());
    }

    /* Create a FunSignature for Operator Overload Call. */
    // Create the arguments.
    atl::vector<atl::shared_ptr<Type>> opOverloadCallArgTypes;
    switch (bo.operation) {
    case Op::ADD:
      opOverloadCallArgTypes.push_back(lhsClassType);
      break;
    default:
      opOverloadCallArgTypes.push_back(
          atl::shared_ptr<PointerType>(new PointerType(lhsClassType)));
    }
    opOverloadCallArgTypes.push_back(rhsType);

    // Create the modifiers.
    atl::set<FunDecl::FunModifiers> lhsModifiers;
    if (lhsType->typeModifiers.find(Type::Modifiers::CONST) ||
        lhsClassType->typeModifiers.find(Type::Modifiers::CONST))
      lhsModifiers.insert(FunDecl::FunModifiers::CONST);

    // Create the Identifier
    const atl::string opOverloadStr = "operator" + opToStr(bo.operation);
    const atl::shared_ptr<Identifier> opOverloadIdentifier(
        new Identifier(opOverloadStr));

    // Construct the Signature.
    const FunSignature opOverloadCallSignature(
        nullptr, opOverloadIdentifier, opOverloadCallArgTypes, lhsModifiers);

    // Try resolve it.
    const atl::shared_ptr<FunDecl> opOverloadClassFunc =
        lhsClassTypeDef->resolveFunCall(opOverloadCallSignature);
    if (opOverloadClassFunc) {
      bo.overload = opOverloadClassFunc;
      return opOverloadClassFunc->funType;
    }

    const atl::shared_ptr<FunDecl> opOverloadScopeFunc =
        currScope->findFunDecl(opOverloadCallSignature);
    if (opOverloadScopeFunc) {
      bo.overload = opOverloadScopeFunc;
      return opOverloadScopeFunc->funType;
    }

    return error("Type Analysis",
                 "No definition for " + opOverloadStr + "(" +
                     lhsType->astClass() + ", " + rhsType->astClass() + ")",
                 bo.getptr());
  }

  if (!lhsType->equivalentTo(*rhsType))
    return error("Type Analysis", "Binary operation has mismatched types.",
                 bo.getptr());

  switch (bo.operation) {
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
  default:
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

  currScope = b.outerScope.lock();
  return noType();
}
atl::shared_ptr<Type> SemanticAnalysis::visit(BoolLiteral &bl) {
  return atl::shared_ptr<BaseType>(new BaseType(PrimitiveType::BOOL));
}
atl::shared_ptr<Type> SemanticAnalysis::visit(CharLiteral &cl) {
  return atl::shared_ptr<BaseType>(new BaseType(PrimitiveType::CHAR));
}
atl::shared_ptr<Type> SemanticAnalysis::visit(ClassType &ct) {
  // Try find a TypeDefDecl for this "ClassType".
  const atl::shared_ptr<TypeDefDecl> tdd =
      currScope->findTypeDefDecl(ct.identifier);
  if (tdd != nullptr) {
    return tdd->type->accept(*this);
  }

  // Try find a ClassTypeDecl for this ClassType.
  const atl::shared_ptr<ClassTypeDecl> ctd =
      currScope->findClassDef(ct.identifier);
  if (ctd != nullptr) {
    ct.typeDefinition = ctd;
    return ct.getptr();
  }

  return error("Name Analysis",
               "Attempted to use a Class that was not declared: " +
                   ct.identifier->toString(),
               ct.getptr());
}
atl::shared_ptr<Type> SemanticAnalysis::visit(ClassTypeDecl &ctd) {
  if (currScope->findClassDef(ctd.getIdentifier(), ctd.getptr()))
    return error("Name Analysis",
                 "Attempted to declare a Class with an identifier that is "
                 "already in use: " +
                     ctd.getIdentifier()->toString(),
                 ctd.getptr());

  return noType();
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
  // parentIdentifiers.push_back(ctd.getIdentifier());

  for (unsigned int idx = 0; idx < ctd.classDecls.size(); ++idx) {
    atl::shared_ptr<Decl> currDecl = ctd.classDecls[idx];
    if (currDecl->astClass() == "FunDef") {
      atl::shared_ptr<FunDecl> funDecl =
          atl::static_pointer_cast<FunDef>(currDecl);
      visit(*funDecl);
    } else if (currDecl->astClass() == "ConstructorDef") {
      atl::shared_ptr<ConstructorDecl> ctorDecl =
          atl::static_pointer_cast<ConstructorDecl>(currDecl);
      visit(*ctorDecl);
    } else if (currDecl->astClass() == "DestructorDef") {
      atl::shared_ptr<DestructorDecl> dtorDecl =
          atl::static_pointer_cast<DestructorDecl>(currDecl);
      visit(*dtorDecl);
    } else {
      currDecl->accept(*this);
    }
  }

  for (unsigned int idx = 0; idx < ctd.classDecls.size(); ++idx) {
    ctd.classDecls[idx]->accept(*this);
  }

  currScope = ctd.outerScope.lock();

  return noType();
}
atl::shared_ptr<Type> SemanticAnalysis::visit(ConstructorCall &cc) {
  // TODO: This should probably resolve the Constructor signature and just
  // search for it.
  const atl::shared_ptr<ClassTypeDef> ctorClassTypeDef =
      currScope->findClassDef(cc.constructorIdentifier);

  if (ctorClassTypeDef == nullptr) {
    return error(
        "Name/Type Analysis",
        "Attempted to call a constructor, but could not resolve the class",
        cc.getptr());
  }

  atl::vector<atl::shared_ptr<Type>> constructorCallArgTypes;
  constructorCallArgTypes.push_back(
      createThisParamType(cc.constructorIdentifier));
  for (unsigned int idx = 0; idx < cc.constructorArgs.size(); ++idx) {
    constructorCallArgTypes.push_back(cc.constructorArgs[idx]->accept(*this));
  }

  atl::shared_ptr<Identifier> ctorIdentifier = cc.constructorIdentifier;
  while (ctorIdentifier->tail()) {
    ctorIdentifier = ctorIdentifier->tail();
  }
  const FunSignature ctorCallSignature(nullptr, ctorIdentifier,
                                       constructorCallArgTypes,
                                       atl::set<FunDecl::FunModifiers>());
  const atl::shared_ptr<ConstructorDecl> ctorDecl =
      ctorClassTypeDef->resolveConstructorCall(ctorCallSignature);
  if (ctorDecl == nullptr)
    return error("Type Analysis", "Attempted to call undeclared constructor.",
                 cc.getptr());

  ++ctorDecl->numCallers;
  cc.constructorDecl = ctorDecl;
  return cc.constructorDecl.lock()->classType;
}
atl::shared_ptr<Type> SemanticAnalysis::visit(ConstructorDecl &cd) {
  cd.outerScope = currScope;
  currScope = cd.getptr();

  for (unsigned int idx = 0; idx < cd.constructorParams.size(); ++idx)
    cd.constructorParams[idx]->accept(*this);

  currScope = cd.outerScope.lock();

  return cd.classType;
}
atl::shared_ptr<Type> SemanticAnalysis::visit(ConstructorDef &cd) {
  cd.outerScope = currScope;
  currScope = cd.getptr();

  cd.classType->accept(*this);

  for (unsigned int idx = 0; idx < cd.constructorParams.size(); ++idx)
    cd.constructorParams[idx]->accept(*this);

  for (unsigned int idx = 0; idx < cd.initialiserList.size(); ++idx) {
    cd.initialiserList[idx]->accept(*this);
    atl::shared_ptr<VarExpr> memberExpr =
        atl::static_pointer_cast<VarExpr>(cd.initialiserList[idx]->lhs);
  }

  cd.constructorBlock->accept(*this);

  currScope = cd.outerScope.lock();

  return cd.classType;
}
atl::shared_ptr<Type> SemanticAnalysis::visit(Deletion &d) {
  d.deletionVar->accept(*this);
  return noType();
}
atl::shared_ptr<Type> SemanticAnalysis::visit(DestructorDecl &dd) {
  return noType();
}
atl::shared_ptr<Type> SemanticAnalysis::visit(DestructorDef &dd) {
  dd.destructorBlock->accept(*this);

  return noType();
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
  return noType();
}
atl::shared_ptr<Type> SemanticAnalysis::visit(EnumClassTypeDecl &ectd) {
  // TODO:
  return noType();
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

  currScope = f.outerScope.lock();
  return noType();
}
atl::shared_ptr<Type> SemanticAnalysis::visit(FunCall &fc) {
  // Check to make sure this isn't a Constructor for a temporary.
  const atl::shared_ptr<ClassTypeDef> classTypeDef =
      currScope->findClassDef(fc.funIdentifier);
  if (classTypeDef != nullptr) {
    atl::vector<atl::shared_ptr<Type>> constructorCallArgTypes;
    constructorCallArgTypes.push_back(createThisParamType(fc.funIdentifier));
    for (unsigned int idx = 0; idx < fc.funArgs.size(); ++idx)
      constructorCallArgTypes.push_back(fc.funArgs[idx]->accept(*this));

    const FunSignature ctorCallSignature(nullptr, fc.funIdentifier,
                                         constructorCallArgTypes,
                                         atl::set<FunDecl::FunModifiers>());
    const atl::shared_ptr<ConstructorDecl> ctorDecl =
        classTypeDef->resolveConstructorCall(ctorCallSignature);
    if (ctorDecl == nullptr)
      return error("Type Analysis", "Attempted to call undeclared constructor.",
                   fc.getptr());

    // Return RVAL Ref to this object.
    return atl::shared_ptr<ReferenceType>(
        new ReferenceType(atl::shared_ptr<ReferenceType>(
            new ReferenceType(ctorDecl->classType))));
  } else {
    atl::vector<atl::shared_ptr<Type>> funCallArgTypes;
    for (unsigned int idx = 0; idx < fc.funArgs.size(); ++idx)
      funCallArgTypes.push_back(fc.funArgs[idx]->accept(*this));

    const FunSignature funCallSignature(nullptr, fc.funIdentifier,
                                        funCallArgTypes,
                                        atl::set<FunDecl::FunModifiers>());
    const atl::shared_ptr<FunDecl> funDecl =
        currScope->findFunDecl(funCallSignature);
    if (funDecl == nullptr) {
      return error("Type Analysis",
                   "Attempted to call undeclared function: " +
                       funCallSignature.toString(),
                   fc.getptr());
    }

    ++funDecl->numCallers;
    fc.funDecl = funDecl;
    return fc.funDecl.lock()->funType;
  }
}
atl::shared_ptr<Type> SemanticAnalysis::visit(FunDecl &fd) {
  if (currScope->findFunDecl(fd.getSignature(), fd.getptr()) ||
      currScope->findVarDecl(fd.getIdentifier(), fd.getptr()))
    return error("Name Analysis",
                 "FunDecl Identifier already in use: " +
                     fd.getIdentifier()->toString(),
                 fd.getptr());

  for (unsigned int idx = 0; idx < fd.funParams.size(); ++idx)
    fd.funParams[idx]->accept(*this);

  fd.funType = fd.funType->accept(*this);
  return fd.funType;
}
atl::shared_ptr<Type> SemanticAnalysis::visit(FunDef &fd) {
  if (currScope->findFunDecl(fd.getSignature(), fd.getptr()) ||
      currScope->findVarDecl(fd.getIdentifier(), fd.getptr()))
    return error("Name Analysis",
                 "FunDef Identifier already in use: " +
                     fd.getIdentifier()->toString(),
                 fd.getIdentifier());

  // currFunDef = fd.getptr();
  fd.outerScope = currScope;
  currScope = fd.getptr();

  for (unsigned int idx = 0; idx < fd.funParams.size(); ++idx)
    fd.funParams[idx]->accept(*this);
  fd.funBlock->accept(*this);

  fd.funType = fd.funType->accept(*this);
  currScope = fd.outerScope.lock();
  return fd.funType;
}
atl::shared_ptr<Type> SemanticAnalysis::visit(Identifier &i) {
  return noType();
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

  return noType();
}
atl::shared_ptr<Type> SemanticAnalysis::visit(IntLiteral &il) {
  if (il.isUnsigned)
    return atl::shared_ptr<BaseType>(new BaseType(PrimitiveType::UINT));
  else
    return atl::shared_ptr<BaseType>(new BaseType(PrimitiveType::INT));
}
atl::shared_ptr<Type> SemanticAnalysis::visit(MemberAccess &ma) {
  atl::shared_ptr<Type> objType = ma.object->accept(*this);
  objType = ReferenceType::collapseReferenceTypes(objType);

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
      objClassType->typeDefinition.lock();
  // Check this ClassType is Defined.
  if (objClassTypeDef == nullptr) {
    return error("Type Error",
                 "Attempted to access member variable of class type that has "
                 "no definition.",
                 ma.object);
  }
  ma.objectTypeDef = objClassTypeDef;

  const atl::shared_ptr<Scope> outerScope = currScope;
  currScope = objClassTypeDef;

  /* Resolve the underlying type */
  const atl::shared_ptr<Type> memberType = ma.fieldVariable->accept(*this);

  currScope = outerScope;

  return memberType;
}
atl::shared_ptr<Type> SemanticAnalysis::visit(MemberCall &mc) {
  atl::shared_ptr<Type> objType = mc.object->accept(*this);
  objType = ReferenceType::collapseReferenceTypes(objType);

  atl::shared_ptr<ClassType> objClassType;
  if (objType->astClass() == "ClassType") {
    if (mc.accessType != SourceToken::Class::DOT) {
      return error("Type Analysis",
                   "Attempted to call member function of class type without "
                   "using `.` operator.",
                   mc.object);
    }
    objClassType = atl::static_pointer_cast<ClassType>(objType);
  } else if (objType->astClass() == "PointerType") {
    if (mc.accessType != SourceToken::Class::PTRDOT)
      return error("Type Analysis",
                   "Attempted to access member variable of class type without "
                   "using `.` operator.",
                   mc.object);
    objType =
        atl::static_pointer_cast<PointerType>(objType)->pointedType->accept(
            *this);
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
      objClassType->typeDefinition.lock();
  // Check this ClassType is Defined.
  if (objClassTypeDef == nullptr) {
    return error("Type Error",
                 "Attempted to call member function of class type that has "
                 "no definition.",
                 mc.object);
  }

  const bool objIsConst =
      objClassType->typeModifiers.find(Type::Modifiers::CONST);
  /* Now Manually Visit the Member Call */
  // Visit all parameters first.
  atl::vector<atl::shared_ptr<Type>> funCallArgTypes;
  funCallArgTypes.push_back(createThisParamType(objClassType->identifier));
  for (unsigned int idx = 0; idx < mc.funCall->funArgs.size(); ++idx)
    funCallArgTypes.push_back(mc.funCall->funArgs[idx]->accept(*this));

  const FunSignature memberFunSignature(nullptr, mc.funCall->funIdentifier,
                                        funCallArgTypes, funModifiers(true));
  atl::shared_ptr<FunDecl> memberFunDecl =
      objClassTypeDef->findFunDeclLocal(memberFunSignature);

  // If the object is not const, try to find  non-const version.
  if (memberFunDecl == nullptr && !objIsConst) {
    memberFunDecl = objClassTypeDef->findFunDeclLocal(
        FunSignature(nullptr, mc.funCall->funIdentifier, funCallArgTypes,
                     funModifiers(false)));
  }

  if (memberFunDecl == nullptr) {
    return error("Type Error",
                 "Attempted to call a member function that does "
                 "not exist in the class definition.",
                 mc.funCall);
  }
  ++memberFunDecl->numCallers;
  mc.funCall->funDecl = memberFunDecl;

  return memberFunDecl->funType;
}
atl::shared_ptr<Type> SemanticAnalysis::visit(Namespace &n) {
  n.outerScope = currScope;
  currScope = n.getptr();
  // parentIdentifiers.push_back(n.getIdentifier());

  for (unsigned int i = 0; i < n.namespaceDecls.size(); ++i) {
    n.namespaceDecls[i]->accept(*this);
    ++n.namespaceDeclsChecked;
  }

  currScope = n.outerScope.lock();
  return noType();
}
atl::shared_ptr<Type> SemanticAnalysis::visit(Not &n) {
  const atl::shared_ptr<Type> notExprType = n.expr->accept(*this);
  if (notExprType->astClass() != "BaseType") {
    const atl::shared_ptr<BaseType> notExprBaseType =
        atl::static_pointer_cast<BaseType>(notExprType);
    if (notExprBaseType->primitiveType != PrimitiveType::BOOL) {
      return error("Name/Type Error", "Cannot negate a non-boolean type.",
                   n.getptr());
    }
  }
  return atl::shared_ptr<BaseType>(new BaseType(PrimitiveType::BOOL));
}
atl::shared_ptr<Type> SemanticAnalysis::visit(Nullptr &n) {
  return atl::shared_ptr<BaseType>(new BaseType(PrimitiveType::NULLPTR_T));
}
atl::shared_ptr<Type> SemanticAnalysis::visit(ParenthExpr &pe) {
  return pe.innerExpr->accept(*this);
}
atl::shared_ptr<Type> SemanticAnalysis::visit(PointerType &pt) {
  pt.pointedType->accept(*this);
  return pt.getptr();
}
atl::shared_ptr<Type> SemanticAnalysis::visit(PrefixOp &po) {
  return po.variable->accept(*this);
}
atl::shared_ptr<Type> SemanticAnalysis::visit(Program &p) {
  currScope = p.getptr();

  parentIdentifiers.push_front(nullptr);

  for (unsigned int idx = 0; idx < p.decls.size(); ++idx) {
    p.decls[idx]->accept(*this);
    ++p.declsChecked;
  }

  return noType();
}
atl::shared_ptr<Type> SemanticAnalysis::visit(ReferenceType &rt) {
  const atl::shared_ptr<Type> referencedType = rt.referencedType->accept(*this);
  rt.referencedType = referencedType;
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
atl::shared_ptr<Type> SemanticAnalysis::visit(StaticCast &sc) {
  sc.expr->accept(*this);
  return sc.type;
}
atl::shared_ptr<Type> SemanticAnalysis::visit(StringLiteral &sl) {
  return atl::shared_ptr<PointerType>(new PointerType(
      atl::shared_ptr<BaseType>(new BaseType(PrimitiveType::CHAR))));
}
atl::shared_ptr<Type> SemanticAnalysis::visit(SubscriptOp &so) {
  atl::shared_ptr<Type> objType = so.variable->accept(*this);
  if (objType == nullptr) {
    return error("Name/Type Error",
                 "Could not resolve type for object: " +
                     so.variable->varIdentifier->toString(),
                 so.variable);
  }
  objType = ReferenceType::collapseReferenceTypes(objType);
  if (objType->astClass() == "ArrayType" ||
      objType->astClass() == "PointerType") {
    const atl::shared_ptr<PointerType> ptrType =
        atl::static_pointer_cast<PointerType>(objType);
    ptrType->accept(*this);
    so.index->accept(*this);
    return ptrType->pointedType;
  } else if (objType->astClass() == "ClassType") {
    const atl::shared_ptr<ClassType> objClassType =
        atl::static_pointer_cast<ClassType>(objType);
    const atl::shared_ptr<ClassTypeDef> objClassTypeDef =
        objClassType->typeDefinition.lock();

    const atl::shared_ptr<Type> indexType = so.index->accept(*this);

    // Create FunSignature for SubscriptOp.
    atl::vector<atl::shared_ptr<Type>> opArgs;
    opArgs.push_back(createThisParamType(objClassType->identifier));
    opArgs.push_back(indexType);
    atl::set<FunDecl::FunModifiers> objTypeModifiers;
    if (objClassType->typeModifiers.find(Type::Modifiers::CONST))
      objTypeModifiers.insert(FunDecl::FunModifiers::CONST);
    const atl::shared_ptr<Identifier> opIdentifier(
        new Identifier("operator[]"));
    const FunSignature opSignature(nullptr, opIdentifier, opArgs,
                                   objTypeModifiers);

    // TODO: Consider if this should search all the scope, or just the scope for
    // the ClassTypeDef already resolved above. The former would allow us to use
    // out of class definitions.
    const atl::shared_ptr<FunDecl> objSubscriptOpDecl =
        objClassTypeDef->findFunDecl(opSignature);
    if (objSubscriptOpDecl == nullptr) {
      return error("Type Error",
                   "No definiton for subscript operator[] for type: " +
                       objClassType->identifier->toString(),
                   so.variable);
    }

    ++objSubscriptOpDecl->numCallers;
    so.operatorDecl = objSubscriptOpDecl;
    return objSubscriptOpDecl->funType;
  } else {
    return error("Type Error",
                 "Cannot perform subscript operator[] on type: " +
                     objType->astClass(),
                 so.variable);
  }
}
atl::shared_ptr<Type> SemanticAnalysis::visit(TemplateDef &td) {
  return nullptr;
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
  return noType();
}
atl::shared_ptr<Type> SemanticAnalysis::visit(TypeDefDecl &tdd) {
  // TODO:
  return noType();
}
atl::shared_ptr<Type> SemanticAnalysis::visit(ValueAt &va) {
  const atl::shared_ptr<Type> exprType = va.derefExpr->accept(*this);
  if (exprType->astClass() != "PointerType")
    return error("Type Analysis",
                 "Attempted to dereference variable that wasn't a pointer. ",
                 va.derefExpr);
  va.exprType = atl::static_pointer_cast<PointerType>(exprType)->pointedType;
  return va.exprType;
}
atl::shared_ptr<Type> SemanticAnalysis::visit(VarDecl &vd) {
  atl::shared_ptr<Type> varType = vd.type->accept(*this);
  if (varType == nullptr) {
    return error("Type Analysis",
                 "Attempted to declare variable " +
                     vd.getIdentifier()->toString() + " with undefined type.",
                 atl::static_pointer_cast<Decl>(vd.getptr()));
  }
  vd.type = varType;
  varType = ReferenceType::collapseReferenceTypes(varType);
  if (varType->astClass() == "ClassType") {
    const atl::shared_ptr<ClassType> vdClassType =
        atl::static_pointer_cast<ClassType>(varType);
    if (vdClassType->typeDefinition.lock() == nullptr) {
      return error("Type Analysis",
                   "Attempted to declare variable '" +
                       vd.getIdentifier()->toString() +
                       "' with undefined class type: " +
                       vdClassType->identifier->toString(),
                   atl::static_pointer_cast<Decl>(vd.getptr()));
    }
  }
  if (currScope->findVarDecl(vd.getIdentifier(), vd.getptr()))
    return error("Name Analysis",
                 "Attempted to declare a Variable with an identifier that is "
                 "already in use: " +
                     vd.getIdentifier()->toString(),
                 atl::static_pointer_cast<Decl>(vd.getptr()));

  return noType();
}
atl::shared_ptr<Type> SemanticAnalysis::visit(VarDef &vd) {
  atl::shared_ptr<Type> varType = vd.type->accept(*this);
  if (varType == nullptr) {
    return error("Type Analysis",
                 "Attempted to define variable " +
                     vd.getIdentifier()->toString() + " with undefined type.",
                 atl::static_pointer_cast<Decl>(vd.getptr()));
  }
  vd.type = varType;
  varType = ReferenceType::collapseReferenceTypes(varType);
  if (varType->astClass() == "ClassType") {
    const atl::shared_ptr<ClassType> vdClassType =
        atl::static_pointer_cast<ClassType>(varType);
    if (vdClassType->typeDefinition.lock() == nullptr) {
      return error("Type Analysis",
                   "Attempted to define variable " +
                       vd.getIdentifier()->toString() +
                       " with undefined class type: " +
                       vdClassType->identifier->toString(),
                   atl::static_pointer_cast<Decl>(vd.getptr()));
    }
  }
  if (currScope->findVarDecl(vd.getIdentifier(), vd.getptr())) {
    return error("Name Analysis",
                 "Attempted to define a Variable with an identifier that is "
                 "already in use: " +
                     vd.getIdentifier()->toString(),
                 atl::static_pointer_cast<Decl>(vd.getptr()));
  }

  // Visit the value initialised.
  const atl::shared_ptr<Type> valueType = vd.varValue->accept(*this);
  if (!valueType->equivalentTo(*varType))
    return error("Type Analysis", "VarDef has mismatched types.",
                 atl::static_pointer_cast<Decl>(vd.getptr()));

  return noType();
}
atl::shared_ptr<Type> SemanticAnalysis::visit(VarExpr &ve) {
  // Compiler may generate VarExpr's and link them.
  if (ve.varDecl.lock() != nullptr)
    return ve.varDecl.lock()->type;
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
  return noType();
}

atl::shared_ptr<PointerType> SemanticAnalysis::createThisParamType(
    atl::shared_ptr<Identifier> identifier) const {
  while (identifier->size() > 1)
    identifier = identifier->tail();
  return atl::shared_ptr<PointerType>(new PointerType(
      atl::shared_ptr<ClassType>(new ClassType(identifier->clone()))));
}

atl::set<FunDecl::FunModifiers>
SemanticAnalysis::funModifiers(bool isConst) const {
  atl::set<FunDecl::FunModifiers> output;
  if (isConst)
    output.insert(FunDecl::FunModifiers::CONST);
  return output;
}

atl::shared_ptr<BaseType> SemanticAnalysis::noType() const {
  return atl::shared_ptr<BaseType>(new BaseType(PrimitiveType::NULLPTR_T));
}