#include "ast/Program.h"
#include "ast/ClassType.h"
#include "ast/ClassTypeDef.h"
#include "ast/FunDef.h"
#include "ast/FunSignature.h"
#include "ast/TemplateDef.h"
#include "ast/VarDef.h"

using namespace ACC;

Program::Program(const atl::vector<atl::shared_ptr<Decl>> &p_decls)
    : decls(p_decls) {
  for (unsigned int i = 0u; i < decls.size(); ++i) {
    const atl::shared_ptr<Decl> currDecl = decls[i];
    if (currDecl->astClass() == "VarDecl" || currDecl->astClass() == "VarDef") {
      globalVars.push_back(currDecl);
    }
  }
}

bool Program::operator==(const Program &rhs) const {
  if (decls.size() != rhs.decls.size())
    return false;

  for (unsigned int i = 0; i < decls.size(); ++i) {
    if (*decls[i] != *rhs.decls[i])
      return false;
  }

  return true;
}

bool Program::operator!=(const Program &rhs) const { return !(*this == rhs); }

atl::shared_ptr<ClassTypeDecl>
Program::findClassDecl(const atl::shared_ptr<Identifier> identifier,
                       const atl::shared_ptr<Decl> &exemptDecl) {
  for (int idx = declsChecked - 1; idx >= 0; --idx) {
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
  if (outerScope.lock() != nullptr)
    return outerScope.lock()->findClassDecl(identifier, exemptDecl);

  return nullptr;
}

atl::shared_ptr<ClassTypeDef>
Program::findClassDef(const atl::shared_ptr<Identifier> identifier,
                      const atl::shared_ptr<Decl> &exemptDecl) {
  if (identifier->size() > 1) {
    for (int idx = declsChecked - 1; idx >= 0; --idx) {
      const atl::shared_ptr<Decl> currDecl = decls[idx];
      if (currDecl->astClass() == "Namespace") {
        const atl::shared_ptr<Namespace> currNamespace =
            atl::static_pointer_cast<Namespace>(currDecl);
        if (currNamespace->identifier->head() != identifier->head())
          continue;

        const atl::shared_ptr<ClassTypeDef> namespaceFind =
            currNamespace->findClassDef(identifier->tail(), exemptDecl);
        if (namespaceFind == nullptr)
          continue;

        return namespaceFind;
      }
    }

    return nullptr;
  } else {
    for (int idx = declsChecked - 1; idx >= 0; --idx) {
      const atl::shared_ptr<Decl> currDecl = decls[idx];
      if (currDecl->astClass() != "ClassTypeDef")
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
}

atl::shared_ptr<FunDecl>
Program::findFunDecl(const FunSignature &funSignature,
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
Program::findFunDeclLocal(const FunSignature &funSignature,
                          const atl::shared_ptr<Decl> &exemptDecl) {
  if (funSignature.namespaceCount() > 0) {
    for (int idx = declsChecked - 1; idx >= 0; --idx) {
      const atl::shared_ptr<Decl> currDecl = decls[idx];
      if (currDecl->astClass() == "ClassTypeDef") {
        const atl::shared_ptr<ClassTypeDef> currClassTypeDef =
            atl::static_pointer_cast<ClassTypeDef>(currDecl);
        if (currClassTypeDef->getIdentifier()->head() !=
            funSignature.namespaceHead())
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
        if (currNamespace->identifier->head() != funSignature.namespaceHead())
          continue;

        const atl::shared_ptr<FunDecl> namespaceFind =
            currNamespace->findFunDeclLocal(funSignature.lowerNamespace(),
                                            exemptDecl);
        if (namespaceFind == nullptr)
          continue;

        return namespaceFind;
      }
    }

    return nullptr;
  } else {
    /* No Namespacing on this FunSignature, search top level. */
    for (int idx = declsChecked - 1; idx >= 0; --idx) {
      const atl::shared_ptr<Decl> currDecl = decls[idx];
      if (currDecl->astClass() == "FunDecl" ||
          currDecl->astClass() == "FunDef") {
        const atl::shared_ptr<FunDecl> currFunDecl =
            atl::static_pointer_cast<FunDecl>(currDecl);
        if (currFunDecl.get() == exemptDecl.get())
          continue;
        if (funSignature != currFunDecl->getSignature())
          continue;

        return currFunDecl;
      } else if (currDecl->astClass() == "TemplateDef") {
        const atl::shared_ptr<TemplateDef> currTemplateDef =
            atl::static_pointer_cast<TemplateDef>(currDecl);
        const atl::shared_ptr<FunDecl> templatedFunc =
            currTemplateDef->findFunDecl(funSignature, exemptDecl);
        if (templatedFunc) {
          return templatedFunc;
        }
      }
    }

    return nullptr;
  }
}

atl::shared_ptr<TypeDefDecl>
Program::findTypeDefDecl(const atl::shared_ptr<Identifier> identifier,
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
Program::findTypeDefDeclLocal(const atl::shared_ptr<Identifier> identifier,
                              const atl::shared_ptr<Decl> &exemptDecl) {
  for (int idx = declsChecked - 1; idx >= 0; --idx) {
    const atl::shared_ptr<Decl> currDecl = decls[idx];
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
Program::findVarDecl(const atl::shared_ptr<Identifier> identifier,
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
Program::findVarDeclLocal(const atl::shared_ptr<Identifier> identifier,
                          const atl::shared_ptr<Decl> &exemptDecl) {
  for (int idx = declsChecked - 1; idx >= 0; --idx) {
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

atl::shared_ptr<FunDecl>
Program::findTemplatedFunDecl(const TemplateFunSignature &FunSignature,
                              const atl::shared_ptr<Decl> &exemptDecl) {
  // TODO: Implement
  return nullptr;
}

atl::shared_ptr<FunDecl>
Program::findTemplatedFunDeclLocal(const TemplateFunSignature &FunSignature,
                                   const atl::shared_ptr<Decl> &exemptDecl) {
  // TODO: Implement
  return nullptr;
}
