#pragma once

#include "Expr.h"
#include "Scope.h"
#include "Type.h"
#include "VarDecl.h"

namespace ACC {

class For : public Stmt,
            public Scope,
            public atl::enable_shared_from_this<For> {

public:
  atl::shared_ptr<VarDecl> initialVarDecl;
  atl::shared_ptr<Expr> condition;
  atl::shared_ptr<Expr> endBodyExpr;
  atl::shared_ptr<Stmt> body;

  For(const atl::shared_ptr<VarDecl> &p_initialVarDecl,
      const atl::shared_ptr<Expr> &p_condition,
      const atl::shared_ptr<Expr> &p_endBodyExpr,
      const atl::shared_ptr<Stmt> &p_body);

  atl::shared_ptr<For> getptr() { return shared_from_this(); }

  atl::string astClass() const override { return "For"; }

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
