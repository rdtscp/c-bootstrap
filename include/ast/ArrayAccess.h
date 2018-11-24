#pragma once

#ifndef ARRAYACCESS_H
#define ARRAYACCESS_H

#include <memory>
#include <string>

#include "Expr.h"

namespace ACC {

class ArrayAccess : public Expr {

public:
  std::shared_ptr<Expr> array;
  std::shared_ptr<Expr> index;

  ArrayAccess(std::shared_ptr<Expr> array, std::shared_ptr<Expr> index)
      : array(array), index(index) {}

  bool operator==(const ArrayAccess &d) const { return true; }

  std::string astClass() const override { return "ArrayAccess"; }
};

}; // namespace ACC

#endif