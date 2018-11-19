#pragma once

#ifndef SIZEOF_H
#define SIZEOF_H

#include <string>

#include "Expr.h"
#include "Type.h"

namespace ACC {

class SizeOf : public Expr {

public:
  std::shared_ptr<Type> type;

  SizeOf(std::shared_ptr<Type> type) : type(type) {
    // Boilerplate Code.
  }

  bool operator==(const SizeOf &d) const { return true; }

  virtual std::string toString() const { return "SizeOf"; }
};

}; // namespace ACC

#endif