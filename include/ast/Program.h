#pragma once

#include "atl/include/unordered_map.h"

#include "ASTNode.h"
#include "ClassTypeDef.h"
#include "FunDef.h"
#include "Namespace.h"
#include "VarDef.h"

namespace ACC {

class Program : public ASTNode,
                public Scope,
                public atl::enable_shared_from_this<Program> {

public:
  unsigned int declsChecked = 0;
  atl::vector<atl::shared_ptr<Decl>> decls;
  atl::vector<atl::shared_ptr<FunDecl>> funDecls;
  atl::vector<atl::shared_ptr<VarDecl>> globalVars;

  Program(const atl::vector<atl::shared_ptr<Decl>> &p_decls);

  atl::shared_ptr<Program> getptr() { return shared_from_this(); }

  atl::string astClass() const override { return "Program"; }

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
};
} // namespace ACC
