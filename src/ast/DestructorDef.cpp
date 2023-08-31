#include "ast/DestructorDef.h"

using namespace ACC;

DestructorDef::DestructorDef(const atl::shared_ptr<Type> &p_classType,
                             const atl::shared_ptr<Block> &p_destructorBlock)
    : DestructorDecl(p_classType), destructorBlock(p_destructorBlock) {}

atl::shared_ptr<Identifier> DestructorDef::getIdentifier() const {
  return classType->identifier;
}

bool DestructorDef::operator==(Decl &rhs) const {
  if (rhs.astClass() == astClass())
    return *this == *static_cast<DestructorDef *>(&rhs);
  return false;
}

bool DestructorDef::operator!=(Decl &rhs) const { return !(*this == rhs); }

bool DestructorDef::operator==(const DestructorDef &rhs) const {
  if (*classType != *rhs.classType)
    return false;

  /* Check Block */
  if (*destructorBlock != *rhs.destructorBlock)
    return false;

  return true;
}

bool DestructorDef::operator!=(const DestructorDef &rhs) const {
  return !(*this == rhs);
}

atl::shared_ptr<ClassTypeDecl>
DestructorDef::findClassDecl(const atl::shared_ptr<Identifier> identifier,
                             const atl::shared_ptr<Decl> &exemptDecl) {
  return outerScope.lock()->findClassDecl(identifier, exemptDecl);
}

atl::shared_ptr<ClassTypeDef>
DestructorDef::findClassDef(const atl::shared_ptr<Identifier> identifier,
                            const atl::shared_ptr<Decl> &exemptDecl) {
  return outerScope.lock()->findClassDef(identifier, exemptDecl);
}

atl::shared_ptr<FunDecl>
DestructorDef::findFunDecl(const FunSignature &funSignature,
                           const atl::shared_ptr<Decl> &exemptDecl) {
  return outerScope.lock()->findFunDecl(funSignature, exemptDecl);
}

atl::shared_ptr<FunDecl>
DestructorDef::findFunDeclLocal(const FunSignature &funSignature,
                                const atl::shared_ptr<Decl> &exemptDecl) {
  return nullptr;
}

atl::shared_ptr<VarDecl>
DestructorDef::findVarDecl(const atl::shared_ptr<Identifier> identifier,
                           const atl::shared_ptr<Decl> &exemptDecl) {
  const atl::shared_ptr<VarDecl> localFind =
      findVarDeclLocal(identifier, exemptDecl);
  if (localFind != nullptr)
    return localFind;
  else if (outerScope.lock() != nullptr)
    return outerScope.lock()->outerScope.lock()->findVarDecl(identifier,
                                                             exemptDecl);
  else
    return nullptr;
}

atl::shared_ptr<VarDecl>
DestructorDef::findVarDeclLocal(const atl::shared_ptr<Identifier> identifier,
                                const atl::shared_ptr<Decl> &exemptDecl) {
  if (thisParam.get() == exemptDecl.get())
    return nullptr;

  if (*thisParam->getIdentifier() != *identifier)
    return nullptr;

  // return nullptr;
  return thisParam;
}