#pragma once

#ifndef ARRAYACCESS_H
#define ARRAYACCESS_H

#include <string>

#include "Expr.h"

namespace ACC {

class ArrayAccess : public Expr {

public:
  std::shared_ptr<Expr> array;
  std::shared_ptr<Expr> index;

  ArrayAccess(std::shared_ptr<Expr> array, std::shared_ptr<Expr> index)
      : array(array), index(index) {
    // Boilerplate Code.
  }

  bool operator==(const ArrayAccess &d) const { return true; }

  virtual std::string toString() const override {
    return array->toString() + "[" + index->toString() + "]";
  }

  std::string strVal() const override { return "ArrayAccess"; }
};

}; // namespace ACC

#endif