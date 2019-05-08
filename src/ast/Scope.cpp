#include "ast/Scope.h"
#include "ast/ClassType.h"
#include "ast/FunDecl.h"

using namespace ACC;

Scope::Scope() : decls({}), outerScope(nullptr) {}

void Scope::insertDecl(const atl::shared_ptr<Decl> &decl) {
  decls.push_back(decl);
}

atl::shared_ptr<ClassTypeDecl>
Scope::resolveClassType(const atl::shared_ptr<ClassType> &type) const {
  for (int idx = decls.size() - 1; idx >= 0; --idx) {
    if (decls[idx]->astClass() == "ClassTypeDecl" ||
        decls[idx]->astClass() == "ClassTypeDef") {
      if (*decls[idx]->getIdentifier() == *type->identifier)
        return decls[idx];
    }
  }
  if (outerScope != nullptr)
    return outerScope->resolveClassType(type);

  return nullptr;
}

atl::shared_ptr<VarDecl>
Scope::resolveVarExpr(const atl::shared_ptr<Identifier> identifier) const {
  for (int idx = decls.size() - 1; idx >= 0; --idx) {
    if (decls[idx]->astClass() == "VarDecl" ||
        decls[idx]->astClass() == "VarDef") {
      if (*decls[idx]->getIdentifier() == *identifier)
        return decls[idx];
    }
  }
  if (outerScope != nullptr)
    return outerScope->resolveVarExpr(identifier);

  return nullptr;
}

atl::shared_ptr<FunDecl>
Scope::resolveFunCall(const atl::string funSignature) const {
  for (int idx = decls.size() - 1; idx >= 0; --idx) {
    if (decls[idx]->astClass() == "FunDecl" ||
        decls[idx]->astClass() == "FunDef") {
      const atl::shared_ptr<FunDecl> currFunDecl =
          atl::static_pointer_cast<FunDecl>(decls[idx]);
      if (currFunDecl->getSignature() == funSignature)
        return currFunDecl;
    }
  }
  if (outerScope != nullptr)
    return outerScope->resolveFunCall(funSignature);

  return nullptr;
}

atl::shared_ptr<Decl>
Scope::duplicateDeclaration(const atl::shared_ptr<Decl> &decl) const {
  const atl::shared_ptr<Decl> &local = duplicateDeclarationLocal(decl);
  if (local == nullptr && outerScope != nullptr)
    return outerScope->duplicateDeclaration(decl);

  return local;
}

atl::shared_ptr<Decl>
Scope::duplicateDeclarationLocal(const atl::shared_ptr<Decl> &decl) const {
  // Reverse iterate decls.
  for (int idx = decls.size() - 1; idx >= 0; --idx) {
    if (*decl == *decls[idx])
      return decls[idx];
  }

  return nullptr;
}