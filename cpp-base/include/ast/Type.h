#pragma once

#ifndef TYPE_H
#define TYPE_H

#include <stdexcept>
#include <string>

#include "ArrayType.h"

namespace ACC {

template <typename Parent, typename Child>
static bool instanceOf(const Child &child) {
  return (typeid(Parent) == typeid(child));
}

class Type {

public:
  Type() {}

  virtual std::string toString() const { return "Type::T"; }

  virtual bool operator==(const Type &t) {
    return (toString() == t.toString());
  }
};

}; // namespace ACC

#endif