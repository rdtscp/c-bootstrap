#pragma once




#include "Expr.h"
#include "VarExpr.h"

namespace ACC {

class PrefixOp : public Expr, public atl::enable_shared_from_this<PrefixOp> {
public:
  enum class Op { INC, DEC };

  Op operation;
  atl::shared_ptr<VarExpr> variable;

  PrefixOp(const Op p_operation, const atl::shared_ptr<VarExpr> &p_variable);
  virtual ~PrefixOp() {}

  atl::shared_ptr<PrefixOp> getptr();

  bool operator==(Expr &rhs) const override;
  bool operator!=(Expr &rhs) const override;

  bool operator==(const PrefixOp &rhs) const;
  bool operator!=(const PrefixOp &rhs) const;

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

