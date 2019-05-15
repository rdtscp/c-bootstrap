#pragma once

#include "Decl.h"
#include "Scope.h"
#include "Stmt.h"
#include "Type.h"

namespace ACC {

class Block : public Stmt,
              public Scope,
              public atl::enable_shared_from_this<Block> {

public:
  unsigned int stmtsChecked = 0;
  atl::vector<atl::shared_ptr<Stmt>> stmts;

  Block(const atl::vector<atl::shared_ptr<Stmt>> &p_stmts);

  bool operator==(const Block &rhs) const;
  bool operator!=(const Block &rhs) const;

  atl::shared_ptr<Block> getptr() { return shared_from_this(); }

  atl::string astClass() const override { return "Block"; }

  /* Scope Methods */
  virtual atl::shared_ptr<ClassTypeDecl> findClassDecl(
      const atl::shared_ptr<Identifier> identifier,
      const atl::shared_ptr<Decl> &exemptDecl = nullptr) const override;

  virtual atl::shared_ptr<ClassTypeDef> findClassDef(
      const atl::shared_ptr<Identifier> identifier,
      const atl::shared_ptr<Decl> &exemptDecl = nullptr) const override;

  virtual atl::shared_ptr<FunDecl>
  findFunDecl(const FunSignature &funSignature,
              const atl::shared_ptr<Decl> &exemptDecl = nullptr) const override;

  virtual atl::shared_ptr<FunDecl> findFunDeclLocal(
      const FunSignature &funSignature,
      const atl::shared_ptr<Decl> &exemptDecl = nullptr) const override;

  virtual atl::shared_ptr<VarDecl>
  findVarDecl(const atl::shared_ptr<Identifier> identifier,
              const atl::shared_ptr<Decl> &exemptDecl = nullptr) const override;

  virtual atl::shared_ptr<VarDecl> findVarDeclLocal(
      const atl::shared_ptr<Identifier> identifier,
      const atl::shared_ptr<Decl> &exemptDecl = nullptr) const override;

  VISITOR_ACCEPTORS
};

} // namespace ACC
