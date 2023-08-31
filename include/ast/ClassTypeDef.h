#pragma once

#include "ClassTypeDecl.h"

namespace ACC {

class ClassTypeDef : public ClassTypeDecl {
public:
  atl::vector<atl::shared_ptr<Decl>> classDecls;

  ClassTypeDef(const atl::shared_ptr<Identifier> &p_classIdentifier,
               const atl::vector<atl::shared_ptr<Decl>> &p_classDecls);

  atl::shared_ptr<Identifier> getIdentifier() const override;

  bool operator==(Decl &rhs) const override;
  bool operator!=(Decl &rhs) const override;

  bool operator==(const ClassTypeDef &rhs) const;
  bool operator!=(const ClassTypeDef &rhs) const;

  atl::shared_ptr<ClassTypeDecl> getptr() {
    return ClassTypeDecl::shared_from_this();
  }

  atl::string astClass() const override { return "ClassTypeDef"; }

  atl::shared_ptr<ConstructorDecl>
  resolveConstructorCall(const FunSignature &ctorSignature,
                         const atl::shared_ptr<Decl> &exemptDecl = nullptr);

  atl::shared_ptr<FunDecl>
  resolveFunCall(const FunSignature &funSignature,
                 const atl::shared_ptr<Decl> &exemptDecl = nullptr);

  /* Scope Methods */
  virtual atl::shared_ptr<ClassTypeDecl>
  findClassDecl(const atl::shared_ptr<Identifier> identifier,
                const atl::shared_ptr<Decl> &exemptDecl = nullptr) override;

  virtual atl::shared_ptr<ClassTypeDef>
  findClassDef(const atl::shared_ptr<Identifier> identifier,
               const atl::shared_ptr<Decl> &exemptDecl = nullptr) override;

  atl::shared_ptr<ConstructorDecl>
  findConstructorDecl(const FunSignature &ctorSignature,
                      const atl::shared_ptr<Decl> &exemptDecl = nullptr);

  virtual atl::shared_ptr<FunDecl>
  findFunDecl(const FunSignature &funSignature,
              const atl::shared_ptr<Decl> &exemptDecl = nullptr) override;

  virtual atl::shared_ptr<FunDecl>
  findFunDeclLocal(const FunSignature &funSignature,
                   const atl::shared_ptr<Decl> &exemptDecl = nullptr) override;

  virtual atl::shared_ptr<TypeDefDecl>
  findTypeDefDecl(const atl::shared_ptr<Identifier> identifier,
                  const atl::shared_ptr<Decl> &exemptDecl = nullptr) override;

  virtual atl::shared_ptr<TypeDefDecl> findTypeDefDeclLocal(
      const atl::shared_ptr<Identifier> identifier,
      const atl::shared_ptr<Decl> &exemptDecl = nullptr) override;

  virtual atl::shared_ptr<VarDecl>
  findVarDecl(const atl::shared_ptr<Identifier> identifier,
              const atl::shared_ptr<Decl> &exemptDecl = nullptr) override;

  virtual atl::shared_ptr<VarDecl>
  findVarDeclLocal(const atl::shared_ptr<Identifier> identifier,
                   const atl::shared_ptr<Decl> &exemptDecl = nullptr) override;

  VISITOR_ACCEPTORS

private:
  void rvalRefWrap(atl::shared_ptr<ConstructorDecl> ctorDecl);
  atl::shared_ptr<VarDecl> createThisParam(const Position &thisPosition) const;
};

} // namespace ACC
