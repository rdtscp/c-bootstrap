#pragma once

#include "atl/include/set.h"
#include "atl/include/shared_ptr.h"
#include "atl/include/string.h"
#include "atl/include/vector.h"

#include "ASTVisitor.h"
#include "Position.h"
#include "targets/X86.h"

namespace ACC {

class Type;

class ASTNode {
public:
  virtual ~ASTNode() {}

  virtual void accept(ASTVisitor<void> &v) = 0;
  virtual atl::string accept(ASTVisitor<atl::string> &v) = 0;
  virtual atl::shared_ptr<Type>
  accept(ASTVisitor<atl::shared_ptr<Type>> &v) = 0;
  virtual atl::shared_ptr<X86::Operand>
  accept(ASTVisitor<atl::shared_ptr<X86::Operand>> &v) = 0;
  virtual atl::shared_ptr<ASTNode>
  accept(ASTVisitor<atl::shared_ptr<ASTNode>> &v) = 0;
  virtual atl::string astClass() const = 0;
};

} // namespace ACC

#define VISITOR_ACCEPTORS                                                      \
  void accept(ASTVisitor<void> &v) override { return v.visit(*this); }         \
  atl::string accept(ASTVisitor<atl::string> &v) override {                    \
    return v.visit(*this);                                                     \
  }                                                                            \
  atl::shared_ptr<Type> accept(ASTVisitor<atl::shared_ptr<Type>> &v)           \
      override {                                                               \
    return v.visit(*this);                                                     \
  }                                                                            \
  atl::shared_ptr<X86::Operand> accept(                                        \
      ASTVisitor<atl::shared_ptr<X86::Operand>> &v) override {                 \
    return v.visit(*this);                                                     \
  }                                                                            \
  atl::shared_ptr<ASTNode> accept(ASTVisitor<atl::shared_ptr<ASTNode>> &v)     \
      override {                                                               \
    return v.visit(*this);                                                     \
  }