#pragma once

#include <string>

#include "PrimitiveType.h"
#include "Type.h"

namespace ACC {

class PointerType : public Type {

public:
  std::shared_ptr<ACC::Type> pointedType;

  PointerType(std::shared_ptr<ACC::Type> type_ptr) {
    pointedType = type_ptr;
  }

  std::string toString() const override {
    return pointedType->toString() + "*";
  }
};

}; // namespace ACC
