#pragma once

#include <string>

#include "PrimitiveType.h"
#include "Type.h"

namespace ACC {

class BaseType : public Type {

public:
  ACC::PrimitiveType primitiveType;

  BaseType(ACC::PrimitiveType pType) : primitiveType(pType) {}

  BaseType(const BaseType& bt) {
    primitiveType = bt.primitiveType;
  }

  std::string toString() const override {
    switch (primitiveType) {
    case ACC::PrimitiveType::INT:
      return "int";
    case ACC::PrimitiveType::CHAR:
      return "char";
    case ACC::PrimitiveType::VOID:
      return "void";
    default:
      return "BaseType::T";
    }
  }
};

}; // namespace ACC
