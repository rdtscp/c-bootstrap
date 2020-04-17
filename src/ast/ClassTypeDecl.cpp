#include "ast/Identifier.h"
#include "ast/ClassTypeDef.h"
#include "ast/FunDef.h"
#include "ast/VarDef.h"

using namespace ACC;

ClassTypeDecl::ClassTypeDecl(const atl::shared_ptr<Identifier> &p_classIdentifier)
    : classIdentifier(p_classIdentifier) {}

atl::shared_ptr<Identifier> ClassTypeDecl::getIdentifier() const {
  return classIdentifier;
}

bool ClassTypeDecl::operator==(Decl &rhs) const {
  if (rhs.astClass() == astClass())
    return *this == *static_cast<ClassTypeDecl *>(&rhs);
  return false;
}

bool ClassTypeDecl::operator!=(Decl &rhs) const { return !(*this == rhs); }

bool ClassTypeDecl::operator==(const ClassTypeDecl &rhs) const {
  if (*classIdentifier != *rhs.classIdentifier)
    return false;

  return true;
}

bool ClassTypeDecl::operator!=(const ClassTypeDecl &rhs) const {
  return !(*this == rhs);
}

atl::shared_ptr<ClassTypeDecl>
ClassTypeDecl::findClassDecl(const atl::shared_ptr<Identifier> identifier,
                             const atl::shared_ptr<Decl> &exemptDecl) {
  return outerScope.lock()->findClassDecl(identifier, exemptDecl);
}

atl::shared_ptr<ClassTypeDef>
ClassTypeDecl::findClassDef(const atl::shared_ptr<Identifier> identifier,
                            const atl::shared_ptr<Decl> &exemptDecl) {
  return outerScope.lock()->findClassDef(identifier, exemptDecl);
}

atl::shared_ptr<FunDecl>
ClassTypeDecl::findFunDecl(const FunSignature &funSignature,
                           const atl::shared_ptr<Decl> &exemptDecl) {
  return outerScope.lock()->findFunDecl(funSignature, exemptDecl);
}

atl::shared_ptr<FunDecl>
ClassTypeDecl::findFunDeclLocal(const FunSignature &funSignature,
                                const atl::shared_ptr<Decl> &exemptDecl) {
  return nullptr;
}

atl::shared_ptr<TypeDefDecl>
ClassTypeDecl::findTypeDefDecl(const atl::shared_ptr<Identifier> identifier,
                               const atl::shared_ptr<Decl> &exemptDecl) {
  return outerScope.lock()->findTypeDefDecl(identifier, exemptDecl);
}

atl::shared_ptr<TypeDefDecl> ClassTypeDecl::findTypeDefDeclLocal(
    const atl::shared_ptr<Identifier> identifier,
    const atl::shared_ptr<Decl> &exemptDecl) {
  return nullptr;
}

atl::shared_ptr<VarDecl>
ClassTypeDecl::findVarDecl(const atl::shared_ptr<Identifier> identifier,
                           const atl::shared_ptr<Decl> &exemptDecl) {
  return outerScope.lock()->findVarDecl(identifier, exemptDecl);
}

atl::shared_ptr<VarDecl>
ClassTypeDecl::findVarDeclLocal(const atl::shared_ptr<Identifier> identifier,
                                const atl::shared_ptr<Decl> &exemptDecl) {
  return nullptr;
}