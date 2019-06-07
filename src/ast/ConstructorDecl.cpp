#include "ast/ConstructorDecl.h"
#include "ast/ClassType.h"
#include "ast/ClassTypeDef.h"
#include "ast/FunDef.h"
#include "ast/FunSignature.h"
#include "ast/VarDef.h"

using namespace ACC;

ConstructorDecl::ConstructorDecl(
    const atl::shared_ptr<Type> &p_classType,
    const atl::vector<atl::shared_ptr<VarDecl>> &p_constructorParams)
    : classType(p_classType), constructorParams(p_constructorParams) {}

atl::shared_ptr<Identifier> ConstructorDecl::getIdentifier() const {
  return classType->identifier;
}

const FunSignature ConstructorDecl::getSignature() const {
  atl::vector<atl::shared_ptr<Type>> paramTypes;
  for (unsigned int idx = 0u; idx < constructorParams.size(); ++idx)
    paramTypes.push_back(constructorParams[idx]->type);

  return FunSignature(classType, classType->identifier, paramTypes,
                      atl::set<FunDecl::FunModifiers>());
}

bool ConstructorDecl::operator==(Decl &rhs) const {
  if (rhs.astClass() == astClass())
    return *this == *static_cast<FunDecl *>(&rhs);
  return false;
}

bool ConstructorDecl::operator!=(Decl &rhs) const { return !(*this == rhs); }

bool ConstructorDecl::operator==(const ConstructorDecl &rhs) const {
  if (*classType != *rhs.classType)
    return false;

  if (constructorParams.size() != rhs.constructorParams.size())
    return false;

  for (unsigned int i = 0u; i < constructorParams.size(); ++i)
    if (*constructorParams[i] != *rhs.constructorParams[i])
      return false;

  return true;
}

bool ConstructorDecl::operator!=(const ConstructorDecl &rhs) const {
  return !(*this == rhs);
}

atl::shared_ptr<ClassTypeDecl>
ConstructorDecl::findClassDecl(const atl::shared_ptr<Identifier> identifier,
                               const atl::shared_ptr<Decl> &exemptDecl) {
  return outerScope->findClassDecl(identifier);
}

atl::shared_ptr<ClassTypeDef>
ConstructorDecl::findClassDef(const atl::shared_ptr<Identifier> identifier,
                              const atl::shared_ptr<Decl> &exemptDecl) {
  return outerScope->findClassDef(identifier);
}

atl::shared_ptr<FunDecl>
ConstructorDecl::findFunDecl(const FunSignature &funSignature,
                             const atl::shared_ptr<Decl> &exemptDecl) {
  return outerScope->findFunDecl(funSignature);
}

atl::shared_ptr<FunDecl>
ConstructorDecl::findFunDeclLocal(const FunSignature &funSignature,
                                  const atl::shared_ptr<Decl> &exemptDecl) {
  return nullptr;
}

atl::shared_ptr<TypeDefDecl>
ConstructorDecl::findTypeDefDecl(const atl::shared_ptr<Identifier> identifier,
                                 const atl::shared_ptr<Decl> &exemptDecl) {
  return outerScope->findTypeDefDecl(identifier, exemptDecl);
}

atl::shared_ptr<TypeDefDecl> ConstructorDecl::findTypeDefDeclLocal(
    const atl::shared_ptr<Identifier> identifier,
    const atl::shared_ptr<Decl> &exemptDecl) {
  return nullptr;
}

atl::shared_ptr<VarDecl>
ConstructorDecl::findVarDecl(const atl::shared_ptr<Identifier> identifier,
                             const atl::shared_ptr<Decl> &exemptDecl) {
  return outerScope->findVarDecl(identifier);
}

atl::shared_ptr<VarDecl>
ConstructorDecl::findVarDeclLocal(const atl::shared_ptr<Identifier> identifier,
                                  const atl::shared_ptr<Decl> &exemptDecl) {
  return nullptr;
}