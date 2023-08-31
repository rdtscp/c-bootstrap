#include "ast/ClassTypeDef.h"
#include "ast/ClassType.h"
#include "ast/ConstructorDef.h"
#include "ast/DestructorDef.h"
#include "ast/FunDef.h"
#include "ast/FunSignature.h"
#include "ast/PointerType.h"
#include "ast/VarDecl.h"

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
      atl::shared_ptr<VarDecl> thisParam =
          createThisParam(ctorDecl->Decl::position);
      ctorDecl->constructorParams.push_front(thisParam);
    }
    if (currDecl->astClass() == "DestructorDecl" ||
        currDecl->astClass() == "DestructorDef") {
      atl::shared_ptr<DestructorDecl> dtorDecl =
          atl::static_pointer_cast<DestructorDecl>(currDecl);

      atl::shared_ptr<VarDecl> thisParam =
          createThisParam(dtorDecl->Decl::position);
      dtorDecl->thisParam = thisParam;
    }
    if (currDecl->astClass() == "FunDecl" || currDecl->astClass() == "FunDef") {
      // Create a new ConstructorDecl as a copy of the original.
      atl::shared_ptr<FunDecl> funDecl =
          atl::static_pointer_cast<FunDecl>(currDecl);
      if (!funDecl->funModifiers.find(FunDecl::FunModifiers::STATIC)) {
        // Create the this param.
        atl::shared_ptr<VarDecl> thisParam =
            createThisParam(funDecl->Decl::position);
        if (funDecl->funModifiers.find(FunDecl::FunModifiers::CONST)) {
          thisParam->type->typeModifiers.insert(Type::Modifiers::CONST);
        }
        funDecl->funParams.push_front(thisParam);
      }
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

  if (funSignature.namespaceCount() > 0) {
    return nullptr;
  } else {
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

atl::shared_ptr<VarDecl>
ClassTypeDef::createThisParam(const Position &thisPosition) const {
  const atl::shared_ptr<ClassType> classType(new ClassType(classIdentifier));
  classType->position = thisPosition;
  const atl::shared_ptr<PointerType> thisType(new PointerType(classType));
  thisType->position = thisPosition;
  const atl::shared_ptr<Identifier> thisIdent(new Identifier("this"));
  thisType->position = thisPosition;
  const atl::shared_ptr<VarDecl> thisParam(new VarDecl(thisType, thisIdent));
  thisParam->Decl::position = thisPosition;
  return thisParam;
}