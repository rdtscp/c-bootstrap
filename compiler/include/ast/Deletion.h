#pragma once

#ifndef ACC_DELETION_H
#define ACC_DELETION_H

#include "Stmt.h"
#include "VarExpr.h"

namespace ACC {

class Deletion : public Stmt, public atl::enable_shared_from_this<Deletion> {

public:
  enum class DelType { OBJECT, ARRAY };

  DelType deletionType;
  atl::shared_ptr<VarExpr> deletionVar;

  Deletion(const DelType deletionType,
           const atl::shared_ptr<VarExpr> &deletionVar);

  virtual ~Deletion() {}

  atl::shared_ptr<Deletion> getptr();

  bool operator==(const Deletion &rhs) const;
  bool operator!=(const Deletion &rhs) const;

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

#endif