#pragma once

#ifndef FIELDACCESS_H
#define FIELDACCESS_H

#include <string>

#include "Expr.h"

namespace ACC {

class FieldAccess : public Expr {

public:
  std::shared_ptr<Expr> object;
  std::string field;

  FieldAccess(std::shared_ptr<Expr> object, std::string field)
      : object(object), field(field) {
    // Boilerplate Code.
  }

  bool operator==(const FieldAccess &d) const { return true; }

  virtual std::string toString() const {
    return "FieldAccess";
  }
};

}; // namespace ACC

#endif