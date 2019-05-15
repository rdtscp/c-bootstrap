#include "ast/Program.h"
#include "ast/ClassType.h"
#include "ast/ClassTypeDef.h"
#include "ast/FunDef.h"
#include "ast/VarDef.h"

using namespace ACC;

Program::Program(const atl::vector<atl::shared_ptr<Decl>> &p_decls)
    : decls(p_decls) {}

atl::shared_ptr<ClassTypeDecl>
Program::findClassDecl(const atl::shared_ptr<Identifier> identifier,
                       const atl::shared_ptr<Decl> &exemptDecl) const {
  for (unsigned int idx = declsChecked - 1u; idx >= 0u; --idx) {
    const atl::shared_ptr<Decl> currDecl = decls[idx];
    if (currDecl->astClass() != "ClassTypeDecl" &&
        currDecl->astClass() != "ClassTypeDef")
      continue;
    if (currDecl.get() == exemptDecl.get())
      continue;
    if (*currDecl->getIdentifier() != *identifier)
      continue;

    return atl::static_pointer_cast<ClassTypeDecl>(currDecl);
  }
  if (outerScope != nullptr)
    return outerScope->findClassDecl(identifier, exemptDecl);

  return nullptr;
}

atl::shared_ptr<ClassTypeDef>
Program::findClassDef(const atl::shared_ptr<Identifier> identifier,
                      const atl::shared_ptr<Decl> &exemptDecl) const {
  for (unsigned int idx = declsChecked - 1u; idx >= 0u; --idx) {
    const atl::shared_ptr<Decl> currDecl = decls[idx];
    if (currDecl->astClass() != "ClassTypeDecl" &&
        currDecl->astClass() != "ClassTypeDef")
      continue;
    if (currDecl.get() == exemptDecl.get())
      continue;
    if (*currDecl->getIdentifier() != *identifier)
      continue;

    return atl::static_pointer_cast<ClassTypeDecl>(currDecl);
  }
  if (outerScope != nullptr)
    return outerScope->findClassDef(identifier, exemptDecl);

  return nullptr;
}

atl::shared_ptr<FunDecl>
Program::findFunDecl(const FunSignature &funSignature,
                     const atl::shared_ptr<Decl> &exemptDecl) const {
  const atl::shared_ptr<FunDecl> localFind =
      findFunDeclLocal(funSignature, exemptDecl);
  if (localFind != nullptr)
    return localFind;
  else if (outerScope != nullptr)
    return outerScope->findFunDecl(funSignature, exemptDecl);
  else
    return nullptr;
}

atl::shared_ptr<FunDecl>
Program::findFunDeclLocal(const FunSignature &funSignature,
                          const atl::shared_ptr<Decl> &exemptDecl) const {
  if (funSignature.namespaceCount() > 0u) {
    for (unsigned int idx = declsChecked - 1u; idx >= 0u; --idx) {
      const atl::shared_ptr<Decl> currDecl = decls[idx];
      if (currDecl->astClass() == "ClassTypeDef") {
        const atl::shared_ptr<ClassTypeDef> currClassTypeDef =
            atl::static_pointer_cast<ClassTypeDef>(currDecl);
        if (*currClassTypeDef->getIdentifier() != *funSignature.namespaceHead())
          continue;
        if (currClassTypeDef.get() == exemptDecl.get())
          continue;

        const atl::shared_ptr<FunDecl> classTypeDefFind =
            currClassTypeDef->findFunDeclLocal(funSignature.lowerNamespace(),
                                               exemptDecl);
        if (classTypeDefFind == nullptr)
          continue;

        return classTypeDefFind;
      } else if (currDecl->astClass() == "Namespace") {
        const atl::shared_ptr<Namespace> currNamespace =
            atl::static_pointer_cast<Namespace>(currDecl);
        if (*currNamespace->identifier != *funSignature.namespaceHead())
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
    for (unsigned int idx = declsChecked - 1u; idx >= 0u; --idx) {
      const atl::shared_ptr<Decl> currDecl = decls[idx];
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

atl::shared_ptr<VarDecl>
Program::findVarDecl(const atl::shared_ptr<Identifier> identifier,
                     const atl::shared_ptr<Decl> &exemptDecl) const {
  const atl::shared_ptr<VarDecl> localFind =
      findVarDeclLocal(identifier, exemptDecl);
  if (localFind != nullptr)
    return localFind;
  else if (outerScope != nullptr)
    return outerScope->findVarDecl(identifier, exemptDecl);
  else
    return nullptr;
}

atl::shared_ptr<VarDecl>
Program::findVarDeclLocal(const atl::shared_ptr<Identifier> identifier,
                          const atl::shared_ptr<Decl> &exemptDecl) const {
  for (unsigned int idx = declsChecked - 1u; idx >= 0u; --idx) {
    const atl::shared_ptr<Decl> currDecl = decls[idx];
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