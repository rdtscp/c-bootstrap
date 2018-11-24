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
      : object(object), field(field) {}

  void accept(ASTVisitor &v) override { v.visit(*this); }
  std::string astClass() const override { return "FieldAccess"; }
};

}; // namespace ACC

#endif