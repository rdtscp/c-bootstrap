#include "ast/ClassTypeDef.h"

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
  for (unsigned int declIdx = 0u; declIdx < classDecls.size(); ++declIdx) {
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
      for (unsigned int paramIdx = 0u;
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
      for (unsigned int paramIdx = 0u;
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
      for (unsigned int paramIdx = 0u; paramIdx < newFunDecl->funParams.size();
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
      for (unsigned int paramIdx = 0u; paramIdx < newFunDef->funParams.size();
           ++paramIdx) {
        newFunParams.push_back(newFunDef->funParams[paramIdx]);
      }

      // Replace the params.
      newFunDef->funParams = newFunParams;

      // Save to this ClassTypeDef.
      classDecls[declIdx] = newFunDef;
    }
    insertDecl(currDecl);
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

  for (unsigned int i = 0u; i < classDecls.size(); ++i)
    if (*classDecls[i] != *rhs.classDecls[i])
      return false;

  return true;
}

bool ClassTypeDef::operator!=(const ClassTypeDef &rhs) const {
  return !(*this == rhs);
}
