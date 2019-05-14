#pragma once

#include "ClassTypeDecl.h"

namespace ACC {

class ClassTypeDef : public ClassTypeDecl {

public:
  atl::vector<atl::shared_ptr<Decl>> classDecls;

  ClassTypeDef(const atl::shared_ptr<ClassType> &p_classType,
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

  /* Scope Methods */
  virtual atl::shared_ptr<ClassTypeDecl> findClassDecl(
      const atl::shared_ptr<Identifier> identifier,
      const atl::shared_ptr<Decl> exemptDecl = nullptr) const override;

  virtual atl::shared_ptr<ClassTypeDef>
  findClassDef(const atl::shared_ptr<Identifier> identifier,
               const atl::shared_ptr<Decl> exemptDecl = nullptr) const override;

  virtual atl::shared_ptr<FunDecl>
  findFunDecl(const atl::string &funSignature,
              const atl::shared_ptr<Decl> exemptDecl = nullptr) const override;

  virtual atl::shared_ptr<FunDecl> findFunDeclLocal(
      const atl::string &funSignature,
      const atl::shared_ptr<Decl> exemptDecl = nullptr) const override;

  virtual atl::shared_ptr<VarDecl>
  findVarDecl(const atl::shared_ptr<Identifier> identifier,
              const atl::shared_ptr<Decl> exemptDecl = nullptr) const override;

  virtual atl::shared_ptr<VarDecl> findVarDeclLocal(
      const atl::shared_ptr<Identifier> identifier,
      const atl::shared_ptr<Decl> exemptDecl = nullptr) const override;

  VISITOR_ACCEPTORS
};

} // namespace ACC
