#pragma once

#include "atl/include/vector.h"

#include "Decl.h"
#include "Scope.h"
#include "Stmt.h"

namespace ACC {

class Block : public Scope,
              public Stmt,
              public atl::enable_shared_from_this<Block> {

public:
  atl::vector<atl::shared_ptr<Stmt>> stmts;

  Block(const atl::vector<atl::shared_ptr<Stmt>> &p_stmts);

  atl::shared_ptr<Block> getptr();

  bool operator==(const Block &rhs) const;
  bool operator!=(const Block &rhs) const;

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
