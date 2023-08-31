#include "ast/For.h"
#include "ast/ClassType.h"
#include "ast/ClassTypeDef.h"
#include "ast/FunDef.h"
#include "ast/VarDef.h"

using namespace ACC;

For::For(const atl::shared_ptr<VarDecl> &p_initialVarDecl,
         const atl::shared_ptr<Expr> &p_condition,
         const atl::shared_ptr<Expr> &p_endBodyExpr,
         const atl::shared_ptr<Stmt> &p_body)
    : initialVarDecl(p_initialVarDecl), condition(p_condition),
      endBodyExpr(p_endBodyExpr), body(p_body) {}

bool For::operator==(const For &rhs) const {
  if (*initialVarDecl != *rhs.initialVarDecl)
    return false;

  if (*condition != *rhs.condition)
    return false;

  if (*endBodyExpr != *rhs.endBodyExpr)
    return false;

  if (*body != *rhs.body)
    return false;

  return true;
}

bool For::operator!=(const For &rhs) const { return !(*this == rhs); }

bool For::operator==(const Stmt &rhs) const {
  if (rhs.astClass() == astClass())
    return *this == *static_cast<const For *>(&rhs);
  return false;
}

bool For::operator!=(const Stmt &rhs) const { return !(*this == rhs); }

atl::shared_ptr<ClassTypeDecl>
For::findClassDecl(const atl::shared_ptr<Identifier> identifier,
                   const atl::shared_ptr<Decl> &exemptDecl) {
  return outerScope.lock()->findClassDecl(identifier, exemptDecl);
}

atl::shared_ptr<ClassTypeDef>
For::findClassDef(const atl::shared_ptr<Identifier> identifier,
                  const atl::shared_ptr<Decl> &exemptDecl) {
  return outerScope.lock()->findClassDef(identifier, exemptDecl);
}

atl::shared_ptr<FunDecl>
For::findFunDecl(const FunSignature &funSignature,
                 const atl::shared_ptr<Decl> &exemptDecl) {
  return outerScope.lock()->findFunDecl(funSignature, exemptDecl);
}

atl::shared_ptr<FunDecl>
For::findFunDeclLocal(const FunSignature &funSignature,
                      const atl::shared_ptr<Decl> &exemptDecl) {
  return nullptr;
}

atl::shared_ptr<TypeDefDecl>
For::findTypeDefDecl(const atl::shared_ptr<Identifier> identifier,
                     const atl::shared_ptr<Decl> &exemptDecl) {
  return outerScope.lock()->findTypeDefDecl(identifier, exemptDecl);
}

atl::shared_ptr<TypeDefDecl>
For::findTypeDefDeclLocal(const atl::shared_ptr<Identifier> identifier,
                          const atl::shared_ptr<Decl> &exemptDecl) {
  return nullptr;
}

atl::shared_ptr<VarDecl>
For::findVarDecl(const atl::shared_ptr<Identifier> identifier,
                 const atl::shared_ptr<Decl> &exemptDecl) {
  if (*initialVarDecl->identifier == *identifier &&
      initialVarDecl.get() != exemptDecl.get())
    return initialVarDecl;
  else
    return outerScope.lock()->findVarDecl(identifier, exemptDecl);
}

atl::shared_ptr<VarDecl>
For::findVarDeclLocal(const atl::shared_ptr<Identifier> identifier,
                      const atl::shared_ptr<Decl> &exemptDecl) {
  return nullptr;
}

atl::shared_ptr<FunDecl>
For::findTemplatedFunDecl(const TemplateFunSignature &FunSignature,
                          const atl::shared_ptr<Decl> &exemptDecl) {
  // TODO: Implement
  return nullptr;
}

atl::shared_ptr<FunDecl>
For::findTemplatedFunDeclLocal(const TemplateFunSignature &FunSignature,
                               const atl::shared_ptr<Decl> &exemptDecl) {
  // TODO: Implement
  return nullptr;
}
