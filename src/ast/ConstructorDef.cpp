#include "ast/ConstructorDef.h"
#include "ast/ClassType.h"
#include "ast/ConstructorDef.h"
#include "ast/FunDef.h"
#include "ast/PointerType.h"
#include "ast/VarDecl.h"

using namespace ACC;

ConstructorDef::ConstructorDef(
    const atl::shared_ptr<Type> &p_classType,
    const atl::vector<atl::shared_ptr<VarDecl>> &p_constructorParams,
    const atl::vector<atl::shared_ptr<Assign>> &p_initialiserList,
    const atl::shared_ptr<Block> &p_constructorBlock)
    : ConstructorDecl(p_classType, p_constructorParams),
      initialiserList(p_initialiserList), constructorBlock(p_constructorBlock) {
  // Make every initialiser list LHS be a MemberAccess.
  for (unsigned int idx = 0u; idx < initialiserList.size(); ++idx) {
    initialiserList[idx]->lhs = createThisAccess(initialiserList[idx]->lhs);
  }
}

atl::shared_ptr<Identifier> ConstructorDef::getIdentifier() const {
  return classType->identifier;
}

atl::shared_ptr<MemberAccess> ConstructorDef::createThisAccess(const atl::shared_ptr<Expr> &memberExpr) const {
  const atl::shared_ptr<VarExpr> thisExpr(new VarExpr(atl::shared_ptr<Identifier>(new Identifier("this"))));
  return atl::shared_ptr<MemberAccess>(new MemberAccess(thisExpr, memberExpr, SourceToken::Class::PTRDOT));
}

bool ConstructorDef::operator==(Decl &rhs) const {
  if (rhs.astClass() == astClass())
    return *this == *static_cast<ConstructorDef *>(&rhs);
  return false;
}

bool ConstructorDef::operator!=(Decl &rhs) const { return !(*this == rhs); }

bool ConstructorDef::operator==(const ConstructorDef &rhs) const {
  if (*classType != *rhs.classType)
    return false;

  if (constructorParams.size() != rhs.constructorParams.size())
    return false;

  for (unsigned int i = 0u; i < constructorParams.size(); ++i)
    if (*constructorParams[i] != *rhs.constructorParams[i])
      return false;

  /* Check Block */
  if (*constructorBlock != *rhs.constructorBlock)
    return false;

  return true;
}

bool ConstructorDef::operator!=(const ConstructorDef &rhs) const {
  return !(*this == rhs);
}

atl::shared_ptr<ClassTypeDecl>
ConstructorDef::findClassDecl(const atl::shared_ptr<Identifier> identifier,
                              const atl::shared_ptr<Decl> &exemptDecl) {
  return outerScope.lock()->findClassDecl(identifier, exemptDecl);
}

atl::shared_ptr<ClassTypeDef>
ConstructorDef::findClassDef(const atl::shared_ptr<Identifier> identifier,
                             const atl::shared_ptr<Decl> &exemptDecl) {
  return outerScope.lock()->findClassDef(identifier, exemptDecl);
}

atl::shared_ptr<FunDecl>
ConstructorDef::findFunDecl(const FunSignature &funSignature,
                            const atl::shared_ptr<Decl> &exemptDecl) {
  return outerScope.lock()->findFunDecl(funSignature, exemptDecl);
}

atl::shared_ptr<FunDecl>
ConstructorDef::findFunDeclLocal(const FunSignature &funSignature,
                                 const atl::shared_ptr<Decl> &exemptDecl) {
  return nullptr;
}

atl::shared_ptr<VarDecl>
ConstructorDef::findVarDecl(const atl::shared_ptr<Identifier> identifier,
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
ConstructorDef::findVarDeclLocal(const atl::shared_ptr<Identifier> identifier,
                                 const atl::shared_ptr<Decl> &exemptDecl) {
  for (unsigned int idx = 0u; idx < constructorParams.size(); ++idx) {
    const atl::shared_ptr<VarDecl> currParam = constructorParams[idx];
    if (currParam.get() == exemptDecl.get())
      continue;
    if (*currParam->getIdentifier() != *identifier)
      continue;

    return currParam;
  }

  return nullptr;
}