#include "ast/Namespace.h"
#include "ast/ClassType.h"
#include "ast/ClassTypeDef.h"
#include "ast/FunDef.h"
#include "ast/FunSignature.h"
#include "ast/VarDef.h"

using namespace ACC;

Namespace::Namespace(const atl::shared_ptr<Identifier> &p_identifier,
                     const atl::vector<atl::shared_ptr<Decl>> &p_namespaceDecls)
    : identifier(p_identifier), namespaceDecls(p_namespaceDecls) {}

atl::shared_ptr<Identifier> Namespace::getIdentifier() const {
  return identifier;
}

bool Namespace::operator==(Decl &rhs) const {
  if (rhs.astClass() == astClass())
    return *this == *static_cast<Namespace *>(&rhs);
  return false;
}

bool Namespace::operator!=(Decl &rhs) const { return !(*this == rhs); }

bool Namespace::operator==(const Namespace &rhs) const {
  if (*identifier != *rhs.identifier)
    return false;

  return true;
}

bool Namespace::operator!=(const Namespace &rhs) const {
  return !(*this == rhs);
}

atl::shared_ptr<ClassTypeDecl>
Namespace::findClassDecl(const atl::shared_ptr<Identifier> identifier,
                         const atl::shared_ptr<Decl> &exemptDecl) {
  // TODO: If a Program/Namespace contains a ClassTypeDef and within said CTD,
  // its members refer to its own type, we won't have finished checking the CTD
  // and so `namespaceDeclsChecked` will not be incremented, and so we won't
  // search our own class.
  for (int idx = namespaceDeclsChecked - 1; idx >= 0; --idx) {
    const atl::shared_ptr<Decl> currDecl = namespaceDecls[idx];
    if (currDecl->astClass() != "ClassTypeDecl" &&
        currDecl->astClass() != "ClassTypeDef")
      continue;
    if (currDecl.get() == exemptDecl.get())
      continue;
    if (*currDecl->getIdentifier() != *identifier)
      continue;

    return atl::static_pointer_cast<ClassTypeDecl>(currDecl);
  }
  if (outerScope.lock() != nullptr)
    return outerScope.lock()->findClassDecl(identifier, exemptDecl);

  return nullptr;
}

atl::shared_ptr<ClassTypeDef>
Namespace::findClassDef(const atl::shared_ptr<Identifier> identifier,
                        const atl::shared_ptr<Decl> &exemptDecl) {
  for (int idx = namespaceDeclsChecked - 1; idx >= 0; --idx) {
    const atl::shared_ptr<Decl> currDecl = namespaceDecls[idx];
    if (currDecl->astClass() != "ClassTypeDecl" &&
        currDecl->astClass() != "ClassTypeDef")
      continue;
    if (currDecl.get() == exemptDecl.get())
      continue;
    if (*currDecl->getIdentifier() != *identifier)
      continue;

    return atl::static_pointer_cast<ClassTypeDecl>(currDecl);
  }
  if (outerScope.lock() != nullptr)
    return outerScope.lock()->findClassDef(identifier, exemptDecl);

  return nullptr;
}

atl::shared_ptr<FunDecl>
Namespace::findFunDecl(const FunSignature &funSignature,
                       const atl::shared_ptr<Decl> &exemptDecl) {
  const atl::shared_ptr<FunDecl> localFind =
      findFunDeclLocal(funSignature, exemptDecl);
  if (localFind != nullptr)
    return localFind;
  else if (outerScope.lock() != nullptr)
    return outerScope.lock()->findFunDecl(funSignature, exemptDecl);
  else
    return nullptr;
}

atl::shared_ptr<FunDecl>
Namespace::findFunDeclLocal(const FunSignature &funSignature,
                            const atl::shared_ptr<Decl> &exemptDecl) {
  if (funSignature.namespaceCount() > 0) {
    for (int idx = namespaceDeclsChecked - 1; idx >= 0; --idx) {
      const atl::shared_ptr<Decl> currDecl = namespaceDecls[idx];
      if (currDecl->astClass() == "ClassTypeDef") {
        const atl::shared_ptr<ClassTypeDef> currClassTypeDef =
            atl::static_pointer_cast<ClassTypeDef>(currDecl);
        if (currClassTypeDef->getIdentifier()->head() !=
            funSignature.namespaceHead())
          continue;

        const atl::shared_ptr<FunDecl> classTypeDefFind =
            currClassTypeDef->findFunDeclLocal(funSignature.lowerNamespace());
        if (classTypeDefFind == nullptr)
          continue;

        return classTypeDefFind;
      } else if (currDecl->astClass() == "Namespace") {
        const atl::shared_ptr<Namespace> currNamespace =
            atl::static_pointer_cast<Namespace>(currDecl);
        if (currNamespace->identifier->head() != funSignature.namespaceHead())
          continue;

        const atl::shared_ptr<FunDecl> namespaceFind =
            currNamespace->findFunDeclLocal(funSignature.lowerNamespace());
        if (namespaceFind == nullptr)
          continue;

        return namespaceFind;
      }
    }

    return nullptr;
  } else {
    /* No Namespacing on this FunSignature, search top level. */
    for (int idx = namespaceDeclsChecked - 1; idx >= 0; --idx) {
      const atl::shared_ptr<Decl> currDecl = namespaceDecls[idx];
      if (currDecl->astClass() != "FunDecl" && currDecl->astClass() != "FunDef")
        continue;
      const atl::shared_ptr<FunDecl> currFunDecl =
          atl::static_pointer_cast<FunDecl>(currDecl);
      if (currFunDecl.get() == exemptDecl.get())
        continue;
      if (funSignature != currFunDecl->getSignature())
        continue;

      return currFunDecl;
    }

    return nullptr;
  }
}

atl::shared_ptr<TypeDefDecl>
Namespace::findTypeDefDecl(const atl::shared_ptr<Identifier> identifier,
                           const atl::shared_ptr<Decl> &exemptDecl) {
  const atl::shared_ptr<TypeDefDecl> localFind =
      findTypeDefDeclLocal(identifier, exemptDecl);
  if (localFind != nullptr)
    return localFind;
  else if (outerScope.lock() != nullptr)
    return outerScope.lock()->findTypeDefDecl(identifier, exemptDecl);
  else
    return nullptr;
}

atl::shared_ptr<TypeDefDecl>
Namespace::findTypeDefDeclLocal(const atl::shared_ptr<Identifier> identifier,
                                const atl::shared_ptr<Decl> &exemptDecl) {
  for (int idx = namespaceDeclsChecked - 1; idx >= 0; --idx) {
    const atl::shared_ptr<Decl> currDecl = namespaceDecls[idx];
    if (currDecl->astClass() != "TypeDefDecl")
      continue;
    if (currDecl.get() == exemptDecl.get())
      continue;
    if (*currDecl->getIdentifier() != *identifier)
      continue;

    return atl::static_pointer_cast<TypeDefDecl>(currDecl);
  }

  return nullptr;
}

atl::shared_ptr<VarDecl>
Namespace::findVarDecl(const atl::shared_ptr<Identifier> identifier,
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
Namespace::findVarDeclLocal(const atl::shared_ptr<Identifier> identifier,
                            const atl::shared_ptr<Decl> &exemptDecl) {
  for (int idx = namespaceDeclsChecked - 1; idx >= 0; --idx) {
    const atl::shared_ptr<Decl> currDecl = namespaceDecls[idx];
    if (currDecl->astClass() != "VarDecl" && currDecl->astClass() != "VarDef")
      continue;
    if (currDecl.get() == exemptDecl.get())
      continue;
    if (*currDecl->getIdentifier() != *identifier)
      continue;

    return atl::static_pointer_cast<VarDecl>(currDecl);
  }

  return nullptr;
}

atl::shared_ptr<FunDecl>
Namespace::findTemplatedFunDecl(const TemplateFunSignature &FunSignature,
                                const atl::shared_ptr<Decl> &exemptDecl) {
  // TODO: Implement
  return nullptr;
}

atl::shared_ptr<FunDecl>
Namespace::findTemplatedFunDeclLocal(const TemplateFunSignature &FunSignature,
                                     const atl::shared_ptr<Decl> &exemptDecl) {
  // TODO: Implement
  return nullptr;
}
