#include "ast/FunDef.h"
#include "ast/ClassType.h"
#include "ast/ConstructorDef.h"
#include "ast/FunDef.h"
#include "ast/PointerType.h"
#include "ast/VarDecl.h"

using namespace ACC;

FunDef::FunDef(const atl::set<FunModifiers> &p_funModifiers,
               const atl::shared_ptr<Identifier> &p_funIdentifier,
               const atl::vector<atl::shared_ptr<VarDecl>> &p_funParams,
               const atl::shared_ptr<Type> &p_funType,
               const atl::shared_ptr<Block> &p_funBlock)
    : FunDecl(p_funModifiers, p_funIdentifier, p_funParams, p_funType),
      funBlock(p_funBlock) {}

atl::shared_ptr<Identifier> FunDef::getIdentifier() const {
  return funIdentifier;
}

bool FunDef::isThisParam(const atl::shared_ptr<VarDecl> &varDecl) const {
  if (varDecl->type->astClass() != "PointerType") {
    return false;
  }

  const atl::shared_ptr<PointerType> varPtrType =
      atl::static_pointer_cast<PointerType>(varDecl->type);
  if (varPtrType->pointedType->astClass() != "ClassType") {
    return false;
  }

  const atl::shared_ptr<Identifier> thisIdent(new Identifier("this"));
  if (*varDecl->identifier != *thisIdent) {
    return false;
  }

  return true;
}

bool FunDef::operator==(Decl &rhs) const {
  if (rhs.astClass() == astClass())
    return *this == *static_cast<FunDef *>(&rhs);
  return false;
}

bool FunDef::operator!=(Decl &rhs) const { return !(*this == rhs); }

bool FunDef::operator==(const FunDecl &rhs) const {
  if (funModifiers != rhs.funModifiers)
    return false;

  if (*funType != *rhs.funType)
    return false;

  if (*funIdentifier != *rhs.funIdentifier)
    return false;

  if (funParams.size() != rhs.funParams.size())
    return false;

  for (unsigned int i = 0; i < funParams.size(); ++i)
    if (*funParams[i]->type != *rhs.funParams[i]->type)
      return false;

  return true;
}

bool FunDef::operator!=(const FunDecl &rhs) const { return !(*this == rhs); }

bool FunDef::operator==(const FunDef &rhs) const {
  if (funModifiers != rhs.funModifiers)
    return false;

  if (*funType != *rhs.funType)
    return false;

  if (*funIdentifier != *rhs.funIdentifier)
    return false;

  if (funParams.size() != rhs.funParams.size())
    return false;

  for (unsigned int i = 0; i < funParams.size(); ++i)
    if (*funParams[i]->type != *rhs.funParams[i]->type)
      return false;

  if (*funBlock != *rhs.funBlock)
    return false;

  return true;
}

bool FunDef::operator!=(const FunDef &rhs) const { return !(*this == rhs); }

atl::shared_ptr<ClassTypeDecl>
FunDef::findClassDecl(const atl::shared_ptr<Identifier> identifier,
                      const atl::shared_ptr<Decl> &exemptDecl) {
  return outerScope.lock()->findClassDecl(identifier, exemptDecl);
}

atl::shared_ptr<ClassTypeDef>
FunDef::findClassDef(const atl::shared_ptr<Identifier> identifier,
                     const atl::shared_ptr<Decl> &exemptDecl) {
  return outerScope.lock()->findClassDef(identifier, exemptDecl);
}

atl::shared_ptr<FunDecl>
FunDef::findFunDecl(const FunSignature &funSignature,
                    const atl::shared_ptr<Decl> &exemptDecl) {
  return outerScope.lock()->findFunDecl(funSignature, exemptDecl);
}

atl::shared_ptr<FunDecl>
FunDef::findFunDeclLocal(const FunSignature &funSignature,
                         const atl::shared_ptr<Decl> &exemptDecl) {
  return nullptr;
}

atl::shared_ptr<VarDecl>
FunDef::findVarDecl(const atl::shared_ptr<Identifier> identifier,
                    const atl::shared_ptr<Decl> &exemptDecl) {
  const atl::shared_ptr<VarDecl> localFind =
      findVarDeclLocal(identifier, exemptDecl);
  if (localFind != nullptr) {
    return localFind;
  } else if (outerScope.lock() != nullptr) {
    if (funParams.size() > 0 && isThisParam(funParams[0])) {
      return outerScope.lock()->outerScope.lock()->findVarDecl(identifier,
                                                               exemptDecl);
    } else {
      return outerScope.lock()->findVarDecl(identifier, exemptDecl);
    }
  } else {
    return nullptr;
  }
}

atl::shared_ptr<VarDecl>
FunDef::findVarDeclLocal(const atl::shared_ptr<Identifier> identifier,
                         const atl::shared_ptr<Decl> &exemptDecl) {
  const int numParams = funParams.size();
  // // Check if
  // if (funParams.size() > 0 && isThisParam(funParams[0])) {
  //   atl::shared_ptr<VarDecl> thisParam = funParams[0];
  //   atl::shared_ptr<ClassTypeDef> thisTypeDef =
  //       atl::static_pointer_cast<ClassType>(thisParam->type)
  //           ->typeDefinition.lock();
  //   const atl::shared_ptr<VarDecl> thisParamFind =
  //       thisTypeDef->findVarDeclLocal(identifier, exemptDecl);
  //   if (thisParamFind != nullptr) {
  //     return thisParamFind;
  //   }
  // }
  for (int idx = 0; idx < numParams; ++idx) {
    const atl::shared_ptr<VarDecl> currParam = funParams[idx];
    if (currParam.get() == exemptDecl.get())
      continue;
    if (*currParam->getIdentifier() != *identifier)
      continue;

    return currParam;
  }

  return nullptr;
}