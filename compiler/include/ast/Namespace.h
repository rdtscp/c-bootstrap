#pragma once

#ifndef ACC_NAMESPACE_H
#define ACC_NAMESPACE_H

#include "../ASTNode.h"
#include "Block.h"

namespace ACC {

class Namespace : public Decl, public atl::enable_shared_from_this<Namespace> {

public:
  atl::string identifier;
  atl::vector<atl::shared_ptr<Decl>> namespaceDecls;

  Namespace(const atl::string &identifier,
            const atl::vector<atl::shared_ptr<Decl>> &namespaceDecls);

  atl::shared_ptr<Namespace> getptr();

  bool operator==(Decl &rhs) const override;
  bool operator!=(Decl &rhs) const override;

  atl::string getIdentifier() const override;

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