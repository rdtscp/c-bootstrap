#pragma once

#include "Expr.h"
#include "Identifier.h"

namespace ACC {

class FunCall : public Expr, public atl::enable_shared_from_this<FunCall> {

public:
  atl::shared_ptr<Identifier> funIdentifier;
  atl::vector<atl::shared_ptr<Expr>> funArgs;
  atl::shared_ptr<FunDecl> funDecl;

  FunCall(const atl::shared_ptr<Identifier> &p_funIdentifier,
          const atl::vector<atl::shared_ptr<Expr>> &p_funArgs);

  atl::shared_ptr<FunCall> getptr();

  bool operator==(Expr &rhs) const override;
  bool operator!=(Expr &rhs) const override;

  bool operator==(const FunCall &rhs) const;
  bool operator!=(const FunCall &rhs) const;

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
