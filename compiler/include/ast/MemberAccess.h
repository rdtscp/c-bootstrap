#pragma once

#include "Expr.h"
#include "Identifier.h"

namespace ACC {

class MemberAccess : public Expr,
                     public atl::enable_shared_from_this<MemberAccess> {

public:
  atl::shared_ptr<Expr> object;
  atl::shared_ptr<Identifier> fieldIdentifier;

  MemberAccess(const atl::shared_ptr<Expr> &p_object,
               const atl::shared_ptr<Identifier> &p_fieldIdentifier);

  atl::shared_ptr<MemberAccess> getptr();

  bool operator==(Expr &rhs) const override;
  bool operator!=(Expr &rhs) const override;

  bool operator==(const MemberAccess &rhs) const;
  bool operator!=(const MemberAccess &rhs) const;

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
