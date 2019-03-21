#pragma once




#include "StructTypeDecl.h"
#include "Type.h"

namespace ACC {

class StructType : public Type,
                   public atl::enable_shared_from_this<StructType> {

public:
  atl::shared_ptr<Identifier> identifier;
  atl::shared_ptr<StructTypeDecl> typeDefinition;

  StructType(const atl::shared_ptr<Identifier> &p_identifier);

  atl::shared_ptr<StructType> getptr();

  int getBytes() const override;
  atl::string getSignature() const override;

  bool operator==(Type &rhs) const override;
  bool operator!=(Type &rhs) const override;

  bool operator==(const StructType &rhs) const;
  bool operator!=(const StructType &rhs) const;

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

