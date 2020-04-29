#include "ast/ClassTypeDef.h"
#include "ast/Block.h"
#include "ast/ClassType.h"
#include "ast/ConstructorCall.h"
#include "ast/ConstructorDef.h"
#include "ast/FunDef.h"
#include "ast/FunSignature.h"
#include "ast/PointerType.h"
#include "ast/ReferenceType.h"
#include "ast/Return.h"
#include "ast/VarDecl.h"
#include "ast/VarDef.h"
#include "ast/VarExpr.h"

using namespace ACC;

ClassTypeDef::ClassTypeDef(
    const atl::shared_ptr<Identifier> &p_classIdentifier,
    const atl::vector<atl::shared_ptr<Decl>> &p_classDecls)
    : ClassTypeDecl(p_classIdentifier), classDecls(p_classDecls) {
  /* Add `this` parameter to all FunDecls. */
  for (unsigned int declIdx = 0; declIdx < classDecls.size(); ++declIdx) {
    const atl::shared_ptr<Decl> currDecl = classDecls[declIdx];
    if (currDecl->astClass() == "ConstructorDecl" ||
        currDecl->astClass() == "ConstructorDef") {
      // Create a new ConstructorDecl as a copy of the original.
      atl::shared_ptr<ConstructorDecl> ctorDecl =
          atl::static_pointer_cast<ConstructorDecl>(currDecl);

      // Create the this param.
      atl::shared_ptr<VarDecl> thisParam = createThisParam(ctorDecl->Decl::position);
      ctorDecl->constructorParams.push_front(thisParam);
    }
    if (currDecl->astClass() == "FunDecl" ||
        currDecl->astClass() == "FunDef") {
      // Create a new ConstructorDecl as a copy of the original.
      atl::shared_ptr<FunDecl> funDecl = atl::static_pointer_cast<FunDecl>(currDecl);

      // Create the this param.
      atl::shared_ptr<VarDecl> thisParam = createThisParam(funDecl->Decl::position);
      if (funDecl->funModifiers.find(FunDecl::FunModifiers::CONST)) {
        thisParam->type->typeModifiers.insert(Type::Modifiers::CONST);
      }
      funDecl->funParams.push_front(thisParam);
    }
    // TODO: Handle Destructor.
  }
}

atl::shared_ptr<Identifier> ClassTypeDef::getIdentifier() const {
  return classIdentifier;
}

bool ClassTypeDef::operator==(Decl &rhs) const {
  if (rhs.astClass() == astClass())
    return *this == *static_cast<ClassTypeDef *>(&rhs);
  return false;
}

bool ClassTypeDef::operator!=(Decl &rhs) const { return !(*this == rhs); }

bool ClassTypeDef::operator==(const ClassTypeDef &rhs) const {
  if (*classIdentifier != *rhs.classIdentifier)
    return false;

  if (classDecls.size() != rhs.classDecls.size())
    return false;

  for (unsigned int i = 0; i < classDecls.size(); ++i)
    if (*classDecls[i] != *rhs.classDecls[i])
      return false;

  return true;
}

bool ClassTypeDef::operator!=(const ClassTypeDef &rhs) const {
  return !(*this == rhs);
}

atl::shared_ptr<ConstructorDecl>
ClassTypeDef::resolveConstructorCall(const FunSignature &ctorSignature,
                                     const atl::shared_ptr<Decl> &exemptDecl) {
  for (int idx = classDecls.size() - 1; idx >= 0; --idx) {
    const atl::shared_ptr<Decl> currDecl = classDecls[idx];
    if (currDecl->astClass() != "ConstructorDecl" &&
        currDecl->astClass() != "ConstructorDef")
      continue;
    const atl::shared_ptr<ConstructorDecl> currCtorDecl =
        atl::static_pointer_cast<ConstructorDecl>(currDecl);
    if (currCtorDecl.get() == exemptDecl.get())
      continue;

    if (!ctorSignature.canCall(currCtorDecl->getSignature()))
      continue;

    return currCtorDecl;
  }

  return nullptr;
}

atl::shared_ptr<FunDecl>
ClassTypeDef::resolveFunCall(const FunSignature &funSignature,
                             const atl::shared_ptr<Decl> &exemptDecl) {
  for (int idx = classDecls.size() - 1; idx >= 0; --idx) {
    const atl::shared_ptr<Decl> currDecl = classDecls[idx];
    if (currDecl->astClass() != "FunDecl" && currDecl->astClass() != "FunDef")
      continue;
    const atl::shared_ptr<FunDecl> currFunDecl =
        atl::static_pointer_cast<FunDecl>(currDecl);
    if (currFunDecl.get() == exemptDecl.get())
      continue;

    if (!funSignature.canCall(currFunDecl->getSignature()))
      continue;

    return currFunDecl;
  }

  return nullptr;
}

atl::shared_ptr<ClassTypeDecl>
ClassTypeDef::findClassDecl(const atl::shared_ptr<Identifier> identifier,
                            const atl::shared_ptr<Decl> &exemptDecl) {
  return outerScope.lock()->findClassDecl(identifier);
}

atl::shared_ptr<ClassTypeDef>
ClassTypeDef::findClassDef(const atl::shared_ptr<Identifier> identifier,
                           const atl::shared_ptr<Decl> &exemptDecl) {
  // A variable in a ClassTypeDef can refer to its own class.
  if (*getIdentifier() == *identifier)
    return getptr();

  for (int idx = classDecls.size() - 1; idx >= 0; --idx) {
    const atl::shared_ptr<Decl> currDecl = classDecls[idx];
    if (currDecl->astClass() != "ClassTypeDecl" &&
        currDecl->astClass() != "ClassTypeDef")
      continue;
    if (currDecl.get() == exemptDecl.get())
      continue;
    if (*currDecl->getIdentifier() != *identifier)
      continue;

    return atl::static_pointer_cast<ClassTypeDecl>(currDecl);
  }
  if (outerScope.lock() != nullptr)
    return outerScope.lock()->findClassDecl(identifier, exemptDecl);

  return nullptr;
}

atl::shared_ptr<ConstructorDecl>
ClassTypeDef::findConstructorDecl(const FunSignature &ctorSignature,
                                  const atl::shared_ptr<Decl> &exemptDecl) {
  for (int idx = classDecls.size() - 1; idx >= 0; --idx) {
    const atl::shared_ptr<Decl> currDecl = classDecls[idx];
    if (currDecl->astClass() != "ConstructorDecl" &&
        currDecl->astClass() != "ConstructorDef")
      continue;
    const atl::shared_ptr<ConstructorDecl> currCtorDecl =
        atl::static_pointer_cast<ConstructorDecl>(currDecl);
    if (currCtorDecl.get() == exemptDecl.get())
      continue;
    if (ctorSignature != currCtorDecl->getSignature())
      continue;

    return currCtorDecl;
  }

  return nullptr;
}

atl::shared_ptr<FunDecl>
ClassTypeDef::findFunDecl(const FunSignature &funSignature,
                          const atl::shared_ptr<Decl> &exemptDecl) {
  const atl::shared_ptr<FunDecl> localFind =
      findFunDeclLocal(funSignature, exemptDecl);
  if (localFind != nullptr)
    return localFind;
  else if (outerScope.lock() != nullptr)
    return outerScope.lock()->findFunDecl(funSignature, exemptDecl);
  else
    return nullptr;
}

atl::shared_ptr<FunDecl>
ClassTypeDef::findFunDeclLocal(const FunSignature &funSignature,
                               const atl::shared_ptr<Decl> &exemptDecl) {
  if (funSignature.namespaceCount() == 1 &&
      getIdentifier()->head() == funSignature.namespaceHead()) {
    for (int idx = classDecls.size() - 1; idx >= 0; --idx) {
      const atl::shared_ptr<Decl> currDecl = classDecls[idx];
      if (currDecl->astClass() != "FunDecl" && currDecl->astClass() != "FunDef")
        continue;
      const atl::shared_ptr<FunDecl> currFunDecl =
          atl::static_pointer_cast<FunDecl>(currDecl);
      if (currFunDecl.get() == exemptDecl.get())
        continue;
      if (funSignature.lowerNamespace() != currFunDecl->getSignature())
        continue;

      return currFunDecl;
    }
  }


    /* No Namespacing on this FunSignature, search top level. */
  for (int idx = classDecls.size() - 1; idx >= 0; --idx) {
    const atl::shared_ptr<Decl> currDecl = classDecls[idx];
    if (currDecl->astClass() != "FunDecl" && currDecl->astClass() != "FunDef")
      continue;
    const atl::shared_ptr<FunDecl> currFunDecl =
        atl::static_pointer_cast<FunDecl>(currDecl);
    if (currFunDecl.get() == exemptDecl.get())
      continue;
    if (funSignature != currFunDecl->getSignature())
      continue;

    return currFunDecl;
  }

  return nullptr;
}

atl::shared_ptr<TypeDefDecl>
ClassTypeDef::findTypeDefDecl(const atl::shared_ptr<Identifier> identifier,
                              const atl::shared_ptr<Decl> &exemptDecl) {
  const atl::shared_ptr<TypeDefDecl> localFind =
      findTypeDefDeclLocal(identifier, exemptDecl);
  if (localFind != nullptr)
    return localFind;
  else if (outerScope.lock() != nullptr)
    return outerScope.lock()->findTypeDefDecl(identifier, exemptDecl);
  else
    return nullptr;
}

atl::shared_ptr<TypeDefDecl>
ClassTypeDef::findTypeDefDeclLocal(const atl::shared_ptr<Identifier> identifier,
                                   const atl::shared_ptr<Decl> &exemptDecl) {
  for (int idx = classDecls.size() - 1; idx >= 0; --idx) {
    const atl::shared_ptr<Decl> currDecl = classDecls[idx];
    if (currDecl->astClass() != "TypeDefDecl")
      continue;
    if (currDecl.get() == exemptDecl.get())
      continue;
    if (*currDecl->getIdentifier() != *identifier)
      continue;

    return atl::static_pointer_cast<TypeDefDecl>(currDecl);
  }

  return nullptr;
}

atl::shared_ptr<VarDecl>
ClassTypeDef::findVarDecl(const atl::shared_ptr<Identifier> identifier,
                          const atl::shared_ptr<Decl> &exemptDecl) {
  const atl::shared_ptr<VarDecl> localFind =
      findVarDeclLocal(identifier, exemptDecl);
  if (localFind != nullptr)
    return localFind;
  else if (outerScope.lock() != nullptr)
    return outerScope.lock()->findVarDecl(identifier, exemptDecl);
  else
    return nullptr;
}

atl::shared_ptr<VarDecl>
ClassTypeDef::findVarDeclLocal(const atl::shared_ptr<Identifier> identifier,
                               const atl::shared_ptr<Decl> &exemptDecl) {
  for (int idx = classDecls.size() - 1; idx >= 0; --idx) {
    const atl::shared_ptr<Decl> currDecl = classDecls[idx];
    if (currDecl->astClass() != "VarDecl" && currDecl->astClass() != "VarDef")
      continue;
    if (currDecl.get() == exemptDecl.get())
      continue;
    if (*currDecl->getIdentifier() != *identifier)
      continue;

    return atl::static_pointer_cast<VarDecl>(currDecl);
  }

  return nullptr;
}

// void ClassTypeDef::rvalRefWrap(atl::shared_ptr<ConstructorDecl> ctorDecl) {
//   // Make this function static.
//   atl::set<FunDecl::FunModifiers> modifiers;
//   modifiers.insert(FunDecl::FunModifiers::STATIC);

//   // Make it return a T&&.
//   const atl::shared_ptr<ReferenceType> returnType(new ReferenceType(
//       atl::shared_ptr<ReferenceType>(new ReferenceType(classType))));

//   // Copy all parameters except the first.
//   atl::vector<atl::shared_ptr<VarDecl>> parameters;
//   for (unsigned int idx = 1u; idx < ctorDecl->constructorParams.size(); ++idx)
//     parameters.push_back(ctorDecl->constructorParams[idx]);

//   atl::vector<atl::shared_ptr<Expr>> arguments;
//   for (unsigned int idx = 0u; idx < parameters.size(); ++idx)
//     arguments.push_back(
//         atl::shared_ptr<VarExpr>(new VarExpr(parameters[idx]->identifier)));

//   const atl::shared_ptr<Identifier> temporaryIdent(new Identifier("temporary"));
//   atl::shared_ptr<ConstructorCall> temporaryConstruction(
//       new ConstructorCall(classType->identifier, arguments));
//   const atl::shared_ptr<VarDef> temporaryVar(
//       new VarDef(classType, temporaryIdent, temporaryConstruction));
//   temporaryConstruction->objectToConstruct = temporaryVar;

//   const atl::shared_ptr<Return> temporaryReturn(
//       new Return(atl::shared_ptr<VarExpr>(new VarExpr(temporaryIdent))));

//   const atl::vector<atl::shared_ptr<Stmt>> funBody = {temporaryVar,
//                                                       temporaryReturn};
//   const atl::shared_ptr<Block> funBlock(new Block(funBody));

//   const atl::shared_ptr<FunDef> funDef(new FunDef(
//       modifiers, classType->identifier, parameters, returnType, funBlock));

//   classDecls.push_back(funDef);
// =======
// atl::shared_ptr<VarDecl> ClassTypeDef::createThisParam(const Position &thisPosition) const {
//   const atl::shared_ptr<ClassType> classType(new ClassType(classIdentifier));
//   classType->position = thisPosition;
//   const atl::shared_ptr<PointerType> thisType(new PointerType(classType));
//   thisType->position = thisPosition;
//   const atl::shared_ptr<Identifier> thisIdent(new Identifier("this"));
//   thisType->position = thisPosition;
//   const atl::shared_ptr<VarDecl> thisParam(new VarDecl(thisType, thisIdent));
//   thisParam->Decl::position = thisPosition;
//   return thisParam;
// }