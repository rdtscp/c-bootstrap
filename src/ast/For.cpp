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