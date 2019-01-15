#pragma once

#ifndef ACC_NAMESPACE_H
#define ACC_NAMESPACE_H

#include "../ASTNode.h"
#include "Block.h"

namespace ACC {

class Namespace : public ASTNode,
                  public std::enable_shared_from_this<Namespace> {

public:
  std::shared_ptr<Block> namespaceBlock;

  Namespace(const std::string &identifier,
            const std::shared_ptr<Block> &namespaceBlock);

  std::shared_ptr<Namespace> getptr();

  void accept(ASTVisitor<void> &v) override;
  std::string accept(ASTVisitor<std::string> &v) override;
  std::shared_ptr<Type> accept(ASTVisitor<std::shared_ptr<Type>> &v) override;
  MIPS::Register accept(ASTVisitor<MIPS::Register> &v) override;
  std::shared_ptr<X86::Operand>
  accept(ASTVisitor<std::shared_ptr<X86::Operand>> &v) override;
  std::shared_ptr<ASTNode>
  accept(ASTVisitor<std::shared_ptr<ASTNode>> &v) override;
  std::string astClass() const override;
};
}; // namespace ACC

#endif