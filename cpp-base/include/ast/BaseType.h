#pragma once

#ifndef BASETYPE_H
#define BASETYPE_H

#include <string>

#include "PrimitiveType.h"
#include "Type.h"

namespace ACC {

class BaseType : public Type {

public:
  PrimitiveType primitiveType;

  BaseType(PrimitiveType pType) : primitiveType(pType) {}

  BaseType(const BaseType& bt) {
    primitiveType = bt.primitiveType;
  }

  std::string toString() const override {
    switch (primitiveType) {
    case PrimitiveType::INT:
      return "int";
    case PrimitiveType::CHAR:
      return "char";
    case PrimitiveType::VOID:
      return "void";
    default:
      return "BaseType::T";
    }
  }
};

}; // namespace ACC

#endif