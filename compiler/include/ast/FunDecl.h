#pragma once

#include "atl/include/set.h"

#include "Decl.h"
#include "Scope.h"
#include "Type.h"
#include "VarDecl.h"

namespace ACC {

class FunDecl : public Decl,
                public Scope,
                public atl::enable_shared_from_this<FunDecl> {

public:
  enum class FunModifiers { CONST, OVERRIDE, STATIC, VIRTUAL };
  atl::set<FunModifiers> funModifiers;
  atl::shared_ptr<Identifier> funIdentifier;
  atl::vector<atl::shared_ptr<VarDecl>> funParams;
  atl::shared_ptr<Type> funType;

  FunDecl(const atl::set<FunModifiers> &p_funModifiers,
          const atl::shared_ptr<Identifier> &p_funIdentifier,
          const atl::vector<atl::shared_ptr<VarDecl>> &p_funParams,
          const atl::shared_ptr<Type> &p_funType);

  FunDecl(const FunDecl &rhs) = delete;
  FunDecl &operator=(const FunDecl &rhs) = delete;
  virtual ~FunDecl() {}

  atl::shared_ptr<FunDecl> getptr();

  bool operator==(Decl &rhs) const override;
  bool operator!=(Decl &rhs) const override;

  bool operator==(const FunDecl &rhs) const;
  bool operator!=(const FunDecl &rhs) const;

  atl::shared_ptr<Identifier> getIdentifier() const override;

  void accept(ASTVisitor<void> &v) override;
  atl::string accept(ASTVisitor<atl::string> &v) override;
  atl::shared_ptr<Type> accept(ASTVisitor<atl::shared_ptr<Type>> &v) override;
  MIPS::Register accept(ASTVisitor<MIPS::Register> &v) override;
  atl::shared_ptr<X86::Operand>
  accept(ASTVisitor<atl::shared_ptr<X86::Operand>> &v) override;
  atl::shared_ptr<ASTNode>
  accept(ASTVisitor<atl::shared_ptr<ASTNode>> &v) override;
  atl::string astClass() const override;
};

} // namespace ACC
