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

  std::string toString() const override { return object->toString() + "." + field; }
  std::string strVal() const override { return "FieldAccess"; }
};

}; // namespace ACC

#endif