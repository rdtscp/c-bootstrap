#pragma once

#include "Decl.h"
#include "Scope.h"

namespace ACC {

class ClassType;

class ClassTypeDecl : public Decl,
                      public Scope,
                      public atl::enable_shared_from_this<ClassTypeDecl> {

public:
  atl::shared_ptr<ClassType> classType;

  ClassTypeDecl(const atl::shared_ptr<ClassType> &p_classType);

  atl::shared_ptr<Identifier> getIdentifier() const override;

  bool operator==(Decl &rhs) const override;
  bool operator!=(Decl &rhs) const override;

  bool operator==(const ClassTypeDecl &rhs) const;
  bool operator!=(const ClassTypeDecl &rhs) const;

  atl::shared_ptr<ClassTypeDecl> getptr() { return shared_from_this(); }

  atl::string astClass() const override { return "ClassTypeDecl"; }

  /* Scope Methods */
  virtual atl::shared_ptr<ClassTypeDecl>
  findClassDecl(const atl::shared_ptr<Identifier> identifier,
                const atl::shared_ptr<Decl> &exemptDecl = nullptr) override;

  virtual atl::shared_ptr<ClassTypeDef>
  findClassDef(const atl::shared_ptr<Identifier> identifier,
               const atl::shared_ptr<Decl> &exemptDecl = nullptr) override;

  virtual atl::shared_ptr<FunDecl>
  findFunDecl(const FunSignature &funSignature,
              const atl::shared_ptr<Decl> &exemptDecl = nullptr) override;

  virtual atl::shared_ptr<FunDecl>
  findFunDeclLocal(const FunSignature &funSignature,
                   const atl::shared_ptr<Decl> &exemptDecl = nullptr) override;

  virtual atl::shared_ptr<VarDecl>
  findVarDecl(const atl::shared_ptr<Identifier> identifier,
              const atl::shared_ptr<Decl> &exemptDecl = nullptr) override;

  virtual atl::shared_ptr<VarDecl>
  findVarDeclLocal(const atl::shared_ptr<Identifier> identifier,
                   const atl::shared_ptr<Decl> &exemptDecl = nullptr) override;

  VISITOR_ACCEPTORS
};

} // namespace ACC
