#include "../../include/ast/ArrayType.h"

using namespace ACC;

ArrayType::ArrayType(atl::shared_ptr<Type> arrayType,
                     const atl::string &arraySize)
    : arraySize(arraySize), arrayType(arrayType) {}

atl::shared_ptr<ArrayType> ArrayType::getptr() { return shared_from_this(); }

bool ArrayType::operator==(Type &rhs) const {
  if (rhs.astClass() == astClass())
    return *this == *static_cast<ArrayType *>(&rhs);
  return false;
}

bool ArrayType::operator!=(Type &t) const { return !(*this == t); }

bool ArrayType::operator==(const ArrayType &rhs) const {
  return (*arrayType == *rhs.arrayType && arraySize == rhs.arraySize);
}

bool ArrayType::operator!=(const ArrayType &rhs) const {
  return !(*this == rhs);
}

int ArrayType::getBytes() const {
  int elementSize = arrayType->getBytes();
  return std::stoi(std::string(arraySize.c_str())) * elementSize;
}

void ArrayType::accept(ASTVisitor<void> &v) { return v.visit(*this); }

atl::string ArrayType::accept(ASTVisitor<atl::string> &v) {
  return v.visit(*this);
}

atl::shared_ptr<Type> ArrayType::accept(ASTVisitor<atl::shared_ptr<Type>> &v) {
  return v.visit(*this);
}

MIPS::Register ArrayType::accept(ASTVisitor<MIPS::Register> &v) {
  return v.visit(*this);
}

atl::shared_ptr<X86::Operand>
ArrayType::accept(ASTVisitor<atl::shared_ptr<X86::Operand>> &v) {
  return v.visit(*this);
}

atl::shared_ptr<ASTNode>
ArrayType::accept(ASTVisitor<atl::shared_ptr<ASTNode>> &v) {
  return v.visit(*this);
}

atl::string ArrayType::astClass() const { return "ArrayType"; }
