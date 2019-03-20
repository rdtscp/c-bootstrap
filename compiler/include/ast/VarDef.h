#pragma once




#include "Decl.h"
#include "Expr.h"
#include "Stmt.h"
#include "Type.h"
#include "VarDecl.h"

namespace ACC {

class VarDef : public VarDecl {

public:
  atl::shared_ptr<Expr> varValue;

  int fpOffset = 0;

  VarDef(const atl::shared_ptr<Type> &p_varType,
         const atl::shared_ptr<Identifier> &p_varIdentifer,
         const atl::shared_ptr<Expr> &p_varValue);

  atl::shared_ptr<VarDecl> getptr();

  bool operator==(Decl &rhs) const override;
  bool operator!=(Decl &rhs) const override;

  bool operator==(const VarDef &rhs) const;
  bool operator!=(const VarDef &rhs) const;

  int getBytes() const;

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

