#pragma once

#include <memory>
#include <string>

#include "Type.h"

namespace ACC {

class ArrayType : public Type {

public:
  std::string arraySize;
  std::shared_ptr<Type> arrayType;

  ArrayType(std::shared_ptr<Type> type_ptr, const std::string& arraySize) : arraySize(arraySize) {
    arrayType = type_ptr;
  }

  std::string toString() const override {
    return arrayType->toString() + "[" + arraySize + "]";
  }
};

}; // namespace ACC
