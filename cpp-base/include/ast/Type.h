#pragma once

#ifndef TYPE_H
#define TYPE_H

#include <string>

namespace ACC {

class Type {

public:
  Type() {}

  virtual std::string toString() const { return "Type::T"; }
};

template <typename Parent, typename Child>
static bool instanceOf(const Child &child) {
  return (typeid(Parent) == typeid(child));
}

}; // namespace ACC

#endif