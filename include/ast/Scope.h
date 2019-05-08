#pragma once

#include "Decl.h"

namespace ACC {

class Scope {
public:
  atl::vector<atl::shared_ptr<Decl>> decls;
  atl::shared_ptr<Scope> outerScope;

  Scope();
  ~Scope() {}

  void insertDecl(const atl::shared_ptr<Decl> &decl);

  atl::shared_ptr<ClassTypeDecl>
  resolveClassType(const atl::shared_ptr<ClassType> &type) const;

  atl::shared_ptr<StructTypeDecl>
  resolveStructType(const atl::shared_ptr<StructType> &type) const;

  atl::shared_ptr<VarDecl>
  resolveVarExpr(const atl::shared_ptr<Identifier> identifier) const;
  atl::shared_ptr<FunDecl> resolveFunCall(const atl::string funSignature) const;

  atl::shared_ptr<Decl>
  duplicateDeclaration(const atl::shared_ptr<Decl> &decl) const;

  atl::shared_ptr<Decl>
  duplicateDeclarationLocal(const atl::shared_ptr<Decl> &decl) const;
};
} // namespace ACC
