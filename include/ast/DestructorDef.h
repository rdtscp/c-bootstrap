
#pragma once

#include "Block.h"
#include "DestructorDecl.h"

namespace ACC {

class DestructorDef : public DestructorDecl {

public:
  atl::shared_ptr<Block> destructorBlock;

  DestructorDef(const atl::shared_ptr<Type> &p_classType,
                const atl::shared_ptr<Block> &p_destructorBlock);

  atl::shared_ptr<Identifier> getIdentifier() const override;

  bool operator==(Decl &rhs) const override;
  bool operator!=(Decl &rhs) const override;

  bool operator==(const DestructorDef &rhs) const;
  bool operator!=(const DestructorDef &rhs) const;

  atl::shared_ptr<DestructorDecl> getptr() {
    return DestructorDecl::shared_from_this();
  }

  atl::string astClass() const override { return "DestructorDef"; }

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
