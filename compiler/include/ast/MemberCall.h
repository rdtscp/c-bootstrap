
#pragma once

#ifndef ACC_MEMBERCALL_H
#define ACC_MEMBERCALL_H

#include "Expr.h"
#include "FunCall.h"

namespace ACC {

class MemberCall : public Expr,
                   public atl::enable_shared_from_this<MemberCall> {

public:
  atl::shared_ptr<Expr> object;
  atl::shared_ptr<FunCall> funCall;
  atl::string field;

  MemberCall(atl::shared_ptr<Expr> object, atl::shared_ptr<FunCall> funCall);

  atl::shared_ptr<MemberCall> getptr();

  bool operator==(Expr &rhs) const override;
  bool operator!=(Expr &rhs) const override;

  bool operator==(const MemberCall &rhs) const;
  bool operator!=(const MemberCall &rhs) const;

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

}; // namespace ACC

#endif