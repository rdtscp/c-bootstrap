#pragma once

#include "Decl.h"
#include "TypeDefDecl.h"

namespace ACC {

class FunSignature;
class TemplateFunSignature;

class Scope {
public:
  atl::weak_ptr<Scope> outerScope;
  atl::vector<atl::shared_ptr<VarDecl>> objectsToDestruct;
  atl::string scopeName;

  Scope() : outerScope(nullptr) {}
  virtual ~Scope() {}

  virtual atl::shared_ptr<ClassTypeDecl>
  findClassDecl(const atl::shared_ptr<Identifier> identifier,
                const atl::shared_ptr<Decl> &exemptDecl = nullptr) = 0;

  virtual atl::shared_ptr<ClassTypeDef>
  findClassDef(const atl::shared_ptr<Identifier> identifier,
               const atl::shared_ptr<Decl> &exemptDecl = nullptr) = 0;

  virtual atl::shared_ptr<FunDecl>
  findFunDecl(const FunSignature &funSignature,
              const atl::shared_ptr<Decl> &exemptDecl = nullptr) = 0;

  virtual atl::shared_ptr<FunDecl>
  findFunDeclLocal(const FunSignature &funSignature,
                   const atl::shared_ptr<Decl> &exemptDecl = nullptr) = 0;

  virtual atl::shared_ptr<TypeDefDecl>
  findTypeDefDecl(const atl::shared_ptr<Identifier> identifier,
                  const atl::shared_ptr<Decl> &exemptDecl = nullptr) = 0;

  virtual atl::shared_ptr<TypeDefDecl>
  findTypeDefDeclLocal(const atl::shared_ptr<Identifier> identifier,
                       const atl::shared_ptr<Decl> &exemptDecl = nullptr) = 0;

  virtual atl::shared_ptr<VarDecl>
  findVarDecl(const atl::shared_ptr<Identifier> identifier,
              const atl::shared_ptr<Decl> &exemptDecl = nullptr) = 0;

  virtual atl::shared_ptr<VarDecl>
  findVarDeclLocal(const atl::shared_ptr<Identifier> identifier,
                   const atl::shared_ptr<Decl> &exemptDecl = nullptr) = 0;

  virtual atl::shared_ptr<FunDecl>
  findTemplatedFunDecl(const TemplateFunSignature &FunSignature,
                       const atl::shared_ptr<Decl> &exemptDecl = nullptr) = 0;

  virtual atl::shared_ptr<FunDecl> findTemplatedFunDeclLocal(
      const TemplateFunSignature &FunSignature,
      const atl::shared_ptr<Decl> &exemptDecl = nullptr) = 0;
};
} // namespace ACC

#define SCOPE_OVERRIDES                                                        \
  virtual atl::shared_ptr<ClassTypeDecl> findClassDecl(                        \
      const atl::shared_ptr<Identifier> identifier,                            \
      const atl::shared_ptr<Decl> &exemptDecl = nullptr) override;             \
                                                                               \
  virtual atl::shared_ptr<ClassTypeDef> findClassDef(                          \
      const atl::shared_ptr<Identifier> identifier,                            \
      const atl::shared_ptr<Decl> &exemptDecl = nullptr) override;             \
                                                                               \
  virtual atl::shared_ptr<FunDecl> findFunDecl(                                \
      const FunSignature &funSignature,                                        \
      const atl::shared_ptr<Decl> &exemptDecl = nullptr) override;             \
                                                                               \
  virtual atl::shared_ptr<FunDecl> findFunDeclLocal(                           \
      const FunSignature &funSignature,                                        \
      const atl::shared_ptr<Decl> &exemptDecl = nullptr) override;             \
                                                                               \
  virtual atl::shared_ptr<TypeDefDecl> findTypeDefDecl(                        \
      const atl::shared_ptr<Identifier> identifier,                            \
      const atl::shared_ptr<Decl> &exemptDecl = nullptr) override;             \
                                                                               \
  virtual atl::shared_ptr<TypeDefDecl> findTypeDefDeclLocal(                   \
      const atl::shared_ptr<Identifier> identifier,                            \
      const atl::shared_ptr<Decl> &exemptDecl = nullptr) override;             \
                                                                               \
  virtual atl::shared_ptr<VarDecl> findVarDecl(                                \
      const atl::shared_ptr<Identifier> identifier,                            \
      const atl::shared_ptr<Decl> &exemptDecl = nullptr) override;             \
                                                                               \
  virtual atl::shared_ptr<VarDecl> findVarDeclLocal(                           \
      const atl::shared_ptr<Identifier> identifier,                            \
      const atl::shared_ptr<Decl> &exemptDecl = nullptr) override;             \
                                                                               \
  virtual atl::shared_ptr<FunDecl> findTemplatedFunDecl(                       \
      const TemplateFunSignature &FunSignature,                                \
      const atl::shared_ptr<Decl> &exemptDecl = nullptr) override;             \
                                                                               \
  virtual atl::shared_ptr<FunDecl> findTemplatedFunDeclLocal(                  \
      const TemplateFunSignature &FunSignature,                                \
      const atl::shared_ptr<Decl> &exemptDecl = nullptr) override;