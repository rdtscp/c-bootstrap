#include "../../include/ast/ArrayType.h"

using namespace ACC;

ArrayType::ArrayType(std::shared_ptr<Type> arrayType,
                     const std::string &arraySize)
    : arraySize(arraySize), arrayType(arrayType) {}

bool ArrayType::operator==(Type &t) const {
  if (t.astClass() == astClass())
    return *this == *static_cast<ArrayType *>(&t);
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
  return std::stoi(arraySize) * elementSize;
}

void ArrayType::accept(ASTVisitor<void> &v) { return v.visit(*this); }

std::string ArrayType::accept(ASTVisitor<std::string> &v) {
  return v.visit(*this);
}

std::shared_ptr<Type> ArrayType::accept(ASTVisitor<std::shared_ptr<Type>> &v) {
  return v.visit(*this);
}

MIPS::Register ArrayType::accept(ASTVisitor<MIPS::Register> &v) {
  return v.visit(*this);
}

std::shared_ptr<X86::Operand> ArrayType::accept(ASTVisitor<std::shared_ptr<X86::Operand>> &v) {
  return v.visit(*this);
}

std::string ArrayType::astClass() const { return "ArrayType"; }
