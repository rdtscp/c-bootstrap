#pragma once

#include "PrimitiveType.h"
#include "Type.h"

namespace ACC {

class PointerType : public Type,
                    public atl::enable_shared_from_this<PointerType> {

public:
  atl::shared_ptr<Type> pointedType;

  PointerType(const atl::shared_ptr<Type> &p_pointedType);

  atl::shared_ptr<PointerType> getptr();

  bool operator==(Type &rhs) const override;

  bool operator!=(Type &rhs) const override;

  bool operator==(const PointerType &rhs) const;
  bool operator!=(const PointerType &rhs) const;

  int getBytes() const override;

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
