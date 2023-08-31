#include "ast/DestructorDecl.h"

using namespace ACC;

DestructorDecl::DestructorDecl(const atl::shared_ptr<Type> &p_classType)
    : classType(p_classType) {}

atl::shared_ptr<Identifier> DestructorDecl::getIdentifier() const {
  return classType->identifier;
}

bool DestructorDecl::operator==(Decl &rhs) const {
  if (rhs.astClass() == astClass())
    return *this == *static_cast<FunDecl *>(&rhs);
  return false;
}

bool DestructorDecl::operator!=(Decl &rhs) const { return !(*this == rhs); }

bool DestructorDecl::operator==(const DestructorDecl &rhs) const {
  if (*classType != *rhs.classType)
    return false;

  return true;
}

bool DestructorDecl::operator!=(const DestructorDecl &rhs) const {
  return !(*this == rhs);
}
atl::shared_ptr<ClassTypeDecl>
DestructorDecl::findClassDecl(const atl::shared_ptr<Identifier> identifier,
                              const atl::shared_ptr<Decl> &exemptDecl) {
  return outerScope.lock()->findClassDecl(identifier);
}

atl::shared_ptr<ClassTypeDef>
DestructorDecl::findClassDef(const atl::shared_ptr<Identifier> identifier,
                             const atl::shared_ptr<Decl> &exemptDecl) {
  return outerScope.lock()->findClassDef(identifier);
}

atl::shared_ptr<FunDecl>
DestructorDecl::findFunDecl(const FunSignature &funSignature,
                            const atl::shared_ptr<Decl> &exemptDecl) {
  return outerScope.lock()->findFunDecl(funSignature);
}

atl::shared_ptr<FunDecl>
DestructorDecl::findFunDeclLocal(const FunSignature &funSignature,
                                 const atl::shared_ptr<Decl> &exemptDecl) {
  return nullptr;
}

atl::shared_ptr<TypeDefDecl>
DestructorDecl::findTypeDefDecl(const atl::shared_ptr<Identifier> identifier,
                                const atl::shared_ptr<Decl> &exemptDecl) {
  return outerScope.lock()->findTypeDefDecl(identifier, exemptDecl);
}

atl::shared_ptr<TypeDefDecl> DestructorDecl::findTypeDefDeclLocal(
    const atl::shared_ptr<Identifier> identifier,
    const atl::shared_ptr<Decl> &exemptDecl) {
  return nullptr;
}

atl::shared_ptr<VarDecl>
DestructorDecl::findVarDecl(const atl::shared_ptr<Identifier> identifier,
                            const atl::shared_ptr<Decl> &exemptDecl) {
  return outerScope.lock()->findVarDecl(identifier);
}

atl::shared_ptr<VarDecl>
DestructorDecl::findVarDeclLocal(const atl::shared_ptr<Identifier> identifier,
                                 const atl::shared_ptr<Decl> &exemptDecl) {
  return nullptr;
}

atl::shared_ptr<FunDecl>
DestructorDecl::findTemplatedFunDecl(const TemplateFunSignature &FunSignature,
                                     const atl::shared_ptr<Decl> &exemptDecl) {
  // TODO: Implement
  return nullptr;
}

atl::shared_ptr<FunDecl> DestructorDecl::findTemplatedFunDeclLocal(
    const TemplateFunSignature &FunSignature,
    const atl::shared_ptr<Decl> &exemptDecl) {
  // TODO: Implement
  return nullptr;
}
