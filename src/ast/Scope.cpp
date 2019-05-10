#include "ast/Scope.h"
#include "ast/ClassType.h"
#include "ast/FunDecl.h"

using namespace ACC;

Scope::Scope() : decls({}), outerScope(nullptr) {}

void Scope::insertDecl(const atl::shared_ptr<Decl> &decl) {
  decls.push_back(decl);
}

atl::shared_ptr<ClassTypeDecl>
Scope::findClassDecl(const atl::shared_ptr<Identifier> identifier) const {
  for (int idx = decls.size() - 1; idx >= 0; --idx) {
    if (decls[idx]->astClass() != "ClassTypeDecl" &&
        decls[idx]->astClass() != "ClassTypeDef")
      continue;
    if (*decls[idx]->getIdentifier() == *identifier) {
      return atl::static_pointer_cast<ClassTypeDecl>(decls[idx]);
    }
  }
  if (outerScope != nullptr)
    return outerScope->findClassDecl(identifier);

  return nullptr;
}

atl::shared_ptr<ClassTypeDef>
Scope::findClassDef(const atl::shared_ptr<Identifier> identifier) const {
  for (int idx = decls.size() - 1; idx >= 0; --idx) {
    if (decls[idx]->astClass() != "ClassTypeDecl" &&
        decls[idx]->astClass() != "ClassTypeDef")
      continue;
    if (*decls[idx]->getIdentifier() == *identifier) {
      return atl::static_pointer_cast<ClassTypeDecl>(decls[idx]);
    }
  }
  if (outerScope != nullptr)
    return outerScope->findClassDef(identifier);

  return nullptr;
}

atl::shared_ptr<FunDecl>
Scope::findFunDecl(const atl::string &funSignature) const {
  const atl::shared_ptr<FunDecl> localFind = findFunDeclLocal(funSignature);
  if (localFind != nullptr)
    return localFind;
  else if (outerScope != nullptr)
    return outerScope->findFunDecl(funSignature);
  else
    return nullptr;
}

atl::shared_ptr<FunDecl>
Scope::findFunDeclLocal(const atl::string &funSignature) const {
  for (int idx = decls.size() - 1; idx >= 0; --idx) {
    if (decls[idx]->astClass() != "FunDecl" &&
        decls[idx]->astClass() != "FunDef")
      continue;
    const atl::shared_ptr<FunDecl> currFunDecl =
        atl::static_pointer_cast<FunDecl>(decls[idx]);
    if (currFunDecl->getSignature() == funSignature) {
      return currFunDecl;
    }
  }

  return nullptr;
}

atl::shared_ptr<VarDecl>
Scope::findVarDecl(const atl::shared_ptr<Identifier> identifier) const {
  const atl::shared_ptr<VarDecl> localFind = findVarDeclLocal(identifier);
  if (localFind != nullptr)
    return localFind;
  else if (outerScope != nullptr)
    return outerScope->findVarDecl(identifier);
  else
    return nullptr;
}

atl::shared_ptr<VarDecl>
Scope::findVarDeclLocal(const atl::shared_ptr<Identifier> identifier) const {
  for (int idx = decls.size() - 1; idx >= 0; --idx) {
    if (decls[idx]->astClass() != "VarDecl" &&
        decls[idx]->astClass() != "VarDef")
      continue;
    if (*decls[idx]->getIdentifier() == *identifier) {
      return atl::static_pointer_cast<VarDecl>(decls[idx]);
    }
  }

  return nullptr;
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