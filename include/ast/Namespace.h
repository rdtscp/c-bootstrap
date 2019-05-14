#pragma once

#include "Block.h"
#include "Scope.h"

namespace ACC {

class Namespace : public Decl,
                  public Scope,
                  public atl::enable_shared_from_this<Namespace> {

public:
  atl::shared_ptr<Identifier> identifier;
  atl::vector<atl::shared_ptr<Decl>> namespaceDecls;

  Namespace(const atl::shared_ptr<Identifier> &p_identifier,
            const atl::vector<atl::shared_ptr<Decl>> &p_namespaceDecls);

  atl::shared_ptr<Identifier> getIdentifier() const override;

  bool operator==(Decl &rhs) const override;
  bool operator!=(Decl &rhs) const override;

  bool operator==(const Namespace &rhs) const;
  bool operator!=(const Namespace &rhs) const;

  atl::shared_ptr<Namespace> getptr() { return shared_from_this(); }

  atl::string astClass() const override { return "Namespace"; }

  /* Scope Methods */
  virtual atl::shared_ptr<ClassTypeDecl> findClassDecl(
      const atl::shared_ptr<Identifier> identifier,
      const atl::shared_ptr<Decl> exemptDecl = nullptr) const override;

  virtual atl::shared_ptr<ClassTypeDef>
  findClassDef(const atl::shared_ptr<Identifier> identifier,
               const atl::shared_ptr<Decl> exemptDecl = nullptr) const override;

  virtual atl::shared_ptr<FunDecl>
  findFunDecl(const FunSignature &funSignature,
              const atl::shared_ptr<Decl> exemptDecl = nullptr) const override;

  virtual atl::shared_ptr<FunDecl> findFunDeclLocal(
      const FunSignature &funSignature,
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
