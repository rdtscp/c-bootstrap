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
                   const atl::shared_ptr<Decl> exemptDecl) const {
  return outerScope->findClassDecl(identifier, exemptDecl);
}

atl::shared_ptr<ClassTypeDef>
For::findClassDef(const atl::shared_ptr<Identifier> identifier,
                  const atl::shared_ptr<Decl> exemptDecl) const {
  return outerScope->findClassDef(identifier, exemptDecl);
}

atl::shared_ptr<FunDecl>
For::findFunDecl(const atl::string &funSignature,
                 const atl::shared_ptr<Decl> exemptDecl) const {
  return outerScope->findFunDecl(funSignature, exemptDecl);
}

atl::shared_ptr<FunDecl>
For::findFunDeclLocal(const atl::string &funSignature,
                      const atl::shared_ptr<Decl> exemptDecl) const {
  return nullptr;
}

atl::shared_ptr<VarDecl>
For::findVarDecl(const atl::shared_ptr<Identifier> identifier,
                 const atl::shared_ptr<Decl> exemptDecl) const {
  if (initialVarDecl->identifier == identifier &&
      initialVarDecl.get() != exemptDecl.get())
    return initialVarDecl;
  else
    return outerScope->findVarDecl(identifier, exemptDecl);
}

atl::shared_ptr<VarDecl>
For::findVarDeclLocal(const atl::shared_ptr<Identifier> identifier,
                      const atl::shared_ptr<Decl> exemptDecl) const {
  return nullptr;
}