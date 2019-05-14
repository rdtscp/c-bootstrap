#include "ast/ClassType.h"
#include "ast/ClassTypeDef.h"
#include "ast/FunDef.h"
#include "ast/VarDef.h"

using namespace ACC;

ClassTypeDecl::ClassTypeDecl(const atl::shared_ptr<ClassType> &p_classType)
    : classType(p_classType) {}

atl::shared_ptr<Identifier> ClassTypeDecl::getIdentifier() const {
  return classType->identifier;
}

bool ClassTypeDecl::operator==(Decl &rhs) const {
  if (rhs.astClass() == astClass())
    return *this == *static_cast<ClassTypeDecl *>(&rhs);
  return false;
}

bool ClassTypeDecl::operator!=(Decl &rhs) const { return !(*this == rhs); }

bool ClassTypeDecl::operator==(const ClassTypeDecl &rhs) const {
  if (*classType != *rhs.classType)
    return false;

  return true;
}

bool ClassTypeDecl::operator!=(const ClassTypeDecl &rhs) const {
  return !(*this == rhs);
}

atl::shared_ptr<ClassTypeDecl>
ClassTypeDecl::findClassDecl(const atl::shared_ptr<Identifier> identifier,
                             const atl::shared_ptr<Decl> exemptDecl) const {
  return outerScope->findClassDecl(identifier, exemptDecl);
}

atl::shared_ptr<ClassTypeDef>
ClassTypeDecl::findClassDef(const atl::shared_ptr<Identifier> identifier,
                            const atl::shared_ptr<Decl> exemptDecl) const {
  return outerScope->findClassDef(identifier, exemptDecl);
}

atl::shared_ptr<FunDecl>
ClassTypeDecl::findFunDecl(const FunSignature &funSignature,
                           const atl::shared_ptr<Decl> exemptDecl) const {
  return outerScope->findFunDecl(funSignature, exemptDecl);
}

atl::shared_ptr<FunDecl>
ClassTypeDecl::findFunDeclLocal(const FunSignature &funSignature,
                                const atl::shared_ptr<Decl> exemptDecl) const {
  return nullptr;
}

atl::shared_ptr<VarDecl>
ClassTypeDecl::findVarDecl(const atl::shared_ptr<Identifier> identifier,
                           const atl::shared_ptr<Decl> exemptDecl) const {
  return outerScope->findVarDecl(identifier, exemptDecl);
}

atl::shared_ptr<VarDecl>
ClassTypeDecl::findVarDeclLocal(const atl::shared_ptr<Identifier> identifier,
                                const atl::shared_ptr<Decl> exemptDecl) const {
  return nullptr;
}