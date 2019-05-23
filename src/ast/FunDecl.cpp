#include "ast/FunDecl.h"
#include "ast/ClassType.h"
#include "ast/ClassTypeDef.h"
#include "ast/FunDef.h"
#include "ast/FunSignature.h"
#include "ast/VarDef.h"

using namespace ACC;

FunDecl::FunDecl(const atl::set<FunModifiers> &p_funModifiers,
                 const atl::shared_ptr<Identifier> &p_funIdentifier,
                 const atl::vector<atl::shared_ptr<VarDecl>> &p_funParams,
                 const atl::shared_ptr<Type> &p_funType)
    : funModifiers(p_funModifiers), funIdentifier(p_funIdentifier),
      funParams(p_funParams), funType(p_funType) {}

atl::shared_ptr<Identifier> FunDecl::getIdentifier() const {
  return funIdentifier;
}

const FunSignature FunDecl::getSignature() const {
  atl::vector<atl::shared_ptr<Type>> paramTypes;
  for (unsigned int idx = 0u; idx < funParams.size(); ++idx)
    paramTypes.push_back(funParams[idx]->type);

  return FunSignature(funType, funIdentifier, paramTypes, funModifiers);
}

bool FunDecl::operator==(Decl &rhs) const {
  if (rhs.astClass() == astClass())
    return *this == *static_cast<FunDecl *>(&rhs);
  return false;
}

bool FunDecl::operator!=(Decl &rhs) const { return !(*this == rhs); }

bool FunDecl::operator==(const FunDecl &rhs) const {
  if (funModifiers != rhs.funModifiers)
    return false;

  if (*funType != *rhs.funType)
    return false;

  if (*funIdentifier != *rhs.funIdentifier)
    return false;

  if (funParams.size() != rhs.funParams.size())
    return false;

  for (unsigned int i = 0u; i < funParams.size(); ++i)
    if (*funParams[i]->type != *rhs.funParams[i]->type)
      return false;

  return true;
}

bool FunDecl::operator!=(const FunDecl &rhs) const { return !(*this == rhs); }

atl::shared_ptr<ClassTypeDecl>
FunDecl::findClassDecl(const atl::shared_ptr<Identifier> identifier,
                       const atl::shared_ptr<Decl> &exemptDecl) {
  return outerScope->findClassDecl(identifier, exemptDecl);
}

atl::shared_ptr<ClassTypeDef>
FunDecl::findClassDef(const atl::shared_ptr<Identifier> identifier,
                      const atl::shared_ptr<Decl> &exemptDecl) {
  return outerScope->findClassDef(identifier, exemptDecl);
}

atl::shared_ptr<FunDecl>
FunDecl::findFunDecl(const FunSignature &funSignature,
                     const atl::shared_ptr<Decl> &exemptDecl) {
  return outerScope->findFunDecl(funSignature, exemptDecl);
}

atl::shared_ptr<FunDecl>
FunDecl::findFunDeclLocal(const FunSignature &funSignature,
                          const atl::shared_ptr<Decl> &exemptDecl) {
  return nullptr;
}

atl::shared_ptr<VarDecl>
FunDecl::findVarDecl(const atl::shared_ptr<Identifier> identifier,
                     const atl::shared_ptr<Decl> &exemptDecl) {
  return outerScope->findVarDecl(identifier, exemptDecl);
}

atl::shared_ptr<VarDecl>
FunDecl::findVarDeclLocal(const atl::shared_ptr<Identifier> identifier,
                          const atl::shared_ptr<Decl> &exemptDecl) {
  return nullptr;
}