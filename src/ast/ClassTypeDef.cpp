#include "ast/ClassTypeDef.h"
#include "ast/ClassType.h"
#include "ast/ConstructorDef.h"
#include "ast/FunDef.h"
#include "ast/PointerType.h"
#include "ast/VarDecl.h"

using namespace ACC;

ClassTypeDef::ClassTypeDef(
    const atl::shared_ptr<ClassType> &p_classType,
    const atl::vector<atl::shared_ptr<Decl>> &p_classDecls)
    : ClassTypeDecl(p_classType), classDecls(p_classDecls) {
  /* Add `this` parameter to all FunDecls. */
  for (unsigned int declIdx = 0; declIdx < classDecls.size(); ++declIdx) {
    const atl::shared_ptr<Decl> currDecl = classDecls[declIdx];
    if (currDecl->astClass() == "ConstructorDecl") {
      // Create a new ConstructorDecl as a copy of the original.
      atl::shared_ptr<ConstructorDecl> newConstructorDecl =
          atl::static_pointer_cast<ConstructorDecl>(currDecl);

      // Create a new vector of the params.
      atl::vector<atl::shared_ptr<VarDecl>> newConstructorParams;

      // Append a hardcoded "this" parameter.
      const Position &thisPosition = newConstructorDecl->position;
      const atl::shared_ptr<PointerType> thisType(new PointerType(classType));
      thisType->position = thisPosition;
      const atl::shared_ptr<Identifier> thisIdent(new Identifier("this"));
      thisType->position = thisPosition;
      const atl::shared_ptr<VarDecl> thisParam(
          new VarDecl(thisType, thisIdent));
      thisParam->Decl::position = thisPosition;
      newConstructorParams.push_back(thisParam);

      // Append all the other params.
      for (unsigned int paramIdx = 0;
           paramIdx < newConstructorDecl->constructorParams.size();
           ++paramIdx) {
        newConstructorParams.push_back(
            newConstructorDecl->constructorParams[paramIdx]);
      }

      // Replace the params.
      newConstructorDecl->constructorParams = newConstructorParams;

      // Save to this ClassTypeDef.
      classDecls[declIdx] = newConstructorDecl;
    }
    if (currDecl->astClass() == "ConstructorDef") {
      // Create a new ConstructorDef as a copy of the original.
      atl::shared_ptr<ConstructorDef> newConstructorDef =
          atl::static_pointer_cast<ConstructorDef>(currDecl);

      // Create a new vector of the params.
      atl::vector<atl::shared_ptr<VarDecl>> newConstructorParams;

      // Append a hardcoded "this" parameter.
      const Position &thisPosition = newConstructorDef->position;
      const atl::shared_ptr<PointerType> thisType(new PointerType(classType));
      thisType->position = thisPosition;
      const atl::shared_ptr<Identifier> thisIdent(new Identifier("this"));
      thisType->position = thisPosition;
      const atl::shared_ptr<VarDecl> thisParam(
          new VarDecl(thisType, thisIdent));
      thisParam->Decl::position = thisPosition;
      newConstructorParams.push_back(thisParam);

      // Append all the other params.
      for (unsigned int paramIdx = 0;
           paramIdx < newConstructorDef->constructorParams.size(); ++paramIdx) {
        newConstructorParams.push_back(
            newConstructorDef->constructorParams[paramIdx]);
      }

      // Replace the params.
      newConstructorDef->constructorParams = newConstructorParams;

      // Save to this ClassTypeDecl.
      classDecls[declIdx] = newConstructorDef;
    }
    if (currDecl->astClass() == "FunDecl") {
      // Create a new FunDecl as a copy of the original.
      atl::shared_ptr<FunDecl> newFunDecl =
          atl::static_pointer_cast<FunDecl>(currDecl);

      // Create a new vector of the params.
      atl::vector<atl::shared_ptr<VarDecl>> newFunParams;

      // Append a hardcoded "this" parameter.
      const Position &thisPosition = newFunDecl->position;
      const atl::shared_ptr<PointerType> thisType(new PointerType(classType));
      thisType->position = thisPosition;
      const atl::shared_ptr<Identifier> thisIdent(new Identifier("this"));
      thisType->position = thisPosition;
      const atl::shared_ptr<VarDecl> thisParam(
          new VarDecl(thisType, thisIdent));
      thisParam->Decl::position = thisPosition;
      newFunParams.push_back(thisParam);

      // Append all the other params.
      for (unsigned int paramIdx = 0; paramIdx < newFunDecl->funParams.size();
           ++paramIdx) {
        newFunParams.push_back(newFunDecl->funParams[paramIdx]);
      }

      // Replace the params.
      newFunDecl->funParams = newFunParams;

      // Save to this ClassTypeDecl.
      classDecls[declIdx] = newFunDecl;
    }
    if (currDecl->astClass() == "FunDef") {
      // Create a new FunDef as a copy of the original.
      atl::shared_ptr<FunDef> newFunDef =
          atl::static_pointer_cast<FunDef>(currDecl);

      // Create a new vector of the params.
      atl::vector<atl::shared_ptr<VarDecl>> newFunParams;

      // Append a hardcoded "this" parameter.
      const Position &thisPosition = newFunDef->position;
      const atl::shared_ptr<PointerType> thisType(new PointerType(classType));
      thisType->position = thisPosition;
      const atl::shared_ptr<Identifier> thisIdent(new Identifier("this"));
      thisType->position = thisPosition;
      const atl::shared_ptr<VarDecl> thisParam(
          new VarDecl(thisType, thisIdent));
      thisParam->Decl::position = thisPosition;
      newFunParams.push_back(thisParam);

      // Append all the other params.
      for (unsigned int paramIdx = 0; paramIdx < newFunDef->funParams.size();
           ++paramIdx) {
        newFunParams.push_back(newFunDef->funParams[paramIdx]);
      }

      // Replace the params.
      newFunDef->funParams = newFunParams;

      // Save to this ClassTypeDef.
      classDecls[declIdx] = newFunDef;
    }
  }
}

atl::shared_ptr<Identifier> ClassTypeDef::getIdentifier() const {
  return classType->identifier;
}

bool ClassTypeDef::operator==(Decl &rhs) const {
  if (rhs.astClass() == astClass())
    return *this == *static_cast<ClassTypeDef *>(&rhs);
  return false;
}

bool ClassTypeDef::operator!=(Decl &rhs) const { return !(*this == rhs); }

bool ClassTypeDef::operator==(const ClassTypeDef &rhs) const {
  if (*classType != *rhs.classType)
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

atl::shared_ptr<ClassTypeDecl>
ClassTypeDef::findClassDecl(const atl::shared_ptr<Identifier> identifier,
                            const atl::shared_ptr<Decl> &exemptDecl) const {
  return outerScope->findClassDecl(identifier);
}

atl::shared_ptr<ClassTypeDef>
ClassTypeDef::findClassDef(const atl::shared_ptr<Identifier> identifier,
                           const atl::shared_ptr<Decl> &exemptDecl) const {
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
  if (outerScope != nullptr)
    return outerScope->findClassDecl(identifier, exemptDecl);

  return nullptr;
}

atl::shared_ptr<FunDecl>
ClassTypeDef::findFunDecl(const FunSignature &funSignature,
                          const atl::shared_ptr<Decl> &exemptDecl) const {
  const atl::shared_ptr<FunDecl> localFind =
      findFunDeclLocal(funSignature, exemptDecl);
  if (localFind != nullptr)
    return localFind;
  else if (outerScope != nullptr)
    return outerScope->findFunDecl(funSignature, exemptDecl);
  else
    return nullptr;
}

atl::shared_ptr<FunDecl>
ClassTypeDef::findFunDeclLocal(const FunSignature &funSignature,
                               const atl::shared_ptr<Decl> &exemptDecl) const {
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

atl::shared_ptr<VarDecl>
ClassTypeDef::findVarDecl(const atl::shared_ptr<Identifier> identifier,
                          const atl::shared_ptr<Decl> &exemptDecl) const {
  const atl::shared_ptr<VarDecl> localFind =
      findVarDeclLocal(identifier, exemptDecl);
  if (localFind != nullptr)
    return localFind;
  else if (outerScope != nullptr)
    return outerScope->findVarDecl(identifier, exemptDecl);
  else
    return nullptr;
}

atl::shared_ptr<VarDecl>
ClassTypeDef::findVarDeclLocal(const atl::shared_ptr<Identifier> identifier,
                               const atl::shared_ptr<Decl> &exemptDecl) const {
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