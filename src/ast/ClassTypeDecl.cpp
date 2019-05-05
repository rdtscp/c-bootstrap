#include "ast/ClassTypeDecl.h"

#include "ast/ConstructorDef.h"
#include "ast/FunDef.h"
#include "ast/PointerType.h"
#include "ast/VarDecl.h"

using namespace ACC;

ClassTypeDecl::ClassTypeDecl(
    const atl::shared_ptr<ClassType> &p_classType,
    const atl::vector<atl::shared_ptr<Decl>> &p_classDecls)
    : classType(p_classType), classDecls(p_classDecls) {
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
      const atl::shared_ptr<VarDecl> thisParam(
          new VarDecl(atl::make_shared<PointerType>(PointerType(classType)),
                      atl::make_shared<Identifier>(Identifier("this"))));
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

      // Save to this ClassTypeDecl.
      classDecls[declIdx] = newConstructorDecl;
    }
    if (currDecl->astClass() == "ConstructorDef") {
      // Create a new ConstructorDef as a copy of the original.
      atl::shared_ptr<ConstructorDef> newConstructorDef =
          atl::static_pointer_cast<ConstructorDef>(currDecl);

      // Create a new vector of the params.
      atl::vector<atl::shared_ptr<VarDecl>> newConstructorParams;

      // Append a hardcoded "this" parameter.
      const atl::shared_ptr<VarDecl> thisParam(
          new VarDecl(atl::make_shared<PointerType>(PointerType(classType)),
                      atl::make_shared<Identifier>(Identifier("this"))));
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
      const atl::shared_ptr<VarDecl> thisParam(
          new VarDecl(atl::make_shared<PointerType>(PointerType(classType)),
                      atl::make_shared<Identifier>(Identifier("this"))));
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
      const atl::shared_ptr<VarDecl> thisParam(
          new VarDecl(atl::make_shared<PointerType>(PointerType(classType)),
                      atl::make_shared<Identifier>(Identifier("this"))));
      newFunParams.push_back(thisParam);

      // Append all the other params.
      for (unsigned int paramIdx = 0; paramIdx < newFunDef->funParams.size();
           ++paramIdx) {
        newFunParams.push_back(newFunDef->funParams[paramIdx]);
      }

      // Replace the params.
      newFunDef->funParams = newFunParams;

      // Save to this ClassTypeDecl.
      classDecls[declIdx] = newFunDef;
    }
    insertDecl(currDecl);
  }
}

atl::shared_ptr<Identifier> ClassTypeDecl::getIdentifier() const {
  return classType->identifier;
}

bool ClassTypeDecl::operator==(Decl &rhs) const {
  if (rhs.astClass() == astClass())
    return *this == *static_cast<ClassTypeDecl *>(&rhs);
  return false;
}

bool ClassTypeDecl::operator!=(Decl &rhs) const { return !(*this == rhs); }

bool ClassTypeDecl::operator==(const ClassTypeDecl &rhs) const {
  if (*classType != *rhs.classType)
    return false;

  if (classDecls.size() != rhs.classDecls.size())
    return false;

  for (unsigned int i = 0; i < classDecls.size(); ++i)
    if (*classDecls[i] != *rhs.classDecls[i])
      return false;

  return true;
}

bool ClassTypeDecl::operator!=(const ClassTypeDecl &rhs) const {
  return !(*this == rhs);
}
