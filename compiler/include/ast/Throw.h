#pragma once

#ifndef ACC_THROW_H
#define ACC_THROW_H

#include "Stmt.h"
#include "StringLiteral.h"

namespace ACC {

class Throw : public Stmt, public atl::enable_shared_from_this<Throw> {

public:
  atl::shared_ptr<StringLiteral> exceptionText;

  Throw(const atl::shared_ptr<StringLiteral> &exceptionText);

  atl::shared_ptr<Throw> getptr();

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