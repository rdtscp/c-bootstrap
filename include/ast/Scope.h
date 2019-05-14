#pragma once

#include "Decl.h"

namespace ACC {

class Scope {
public:
  atl::shared_ptr<Scope> outerScope;

  Scope() : outerScope(nullptr) {}
  virtual ~Scope() {}

  virtual atl::shared_ptr<ClassTypeDecl>
  findClassDecl(const atl::shared_ptr<Identifier> identifier,
                const atl::shared_ptr<Decl> exemptDecl = nullptr) const = 0;

  virtual atl::shared_ptr<ClassTypeDef>
  findClassDef(const atl::shared_ptr<Identifier> identifier,
               const atl::shared_ptr<Decl> exemptDecl = nullptr) const = 0;

  virtual atl::shared_ptr<FunDecl>
  findFunDecl(const atl::string &funSignature,
              const atl::shared_ptr<Decl> exemptDecl = nullptr) const = 0;

  virtual atl::shared_ptr<FunDecl>
  findFunDeclLocal(const atl::string &funSignature,
                   const atl::shared_ptr<Decl> exemptDecl = nullptr) const = 0;

  virtual atl::shared_ptr<VarDecl>
  findVarDecl(const atl::shared_ptr<Identifier> identifier,
              const atl::shared_ptr<Decl> exemptDecl = nullptr) const = 0;

  virtual atl::shared_ptr<VarDecl>
  findVarDeclLocal(const atl::shared_ptr<Identifier> identifier,
                   const atl::shared_ptr<Decl> exemptDecl = nullptr) const = 0;
};
} // namespace ACC
