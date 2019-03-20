#pragma once




#include "Expr.h"

namespace ACC {

class VarExpr : public Expr, public atl::enable_shared_from_this<VarExpr> {

public:
  atl::shared_ptr<Identifier> varIdentifier;
  atl::shared_ptr<VarDecl> varDecl;

  VarExpr(const atl::shared_ptr<Identifier> &p_varIdentifier);

  atl::shared_ptr<VarExpr> getptr();

  bool operator==(Expr &rhs) const override;
  bool operator!=(Expr &rhs) const override;

  bool operator==(const VarExpr &rhs) const;
  bool operator!=(const VarExpr &rhs) const;

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

