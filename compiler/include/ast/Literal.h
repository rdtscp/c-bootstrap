#pragma once

#ifndef ACC_LITERAL_H
#define ACC_LITERAL_H

#include "Expr.h"

namespace ACC {

class Literal : public Expr {
public:
  const atl::string value;

  Literal(const atl::string &value) : value(value) {}
  virtual ~Literal() {}
  virtual bool operator==(Expr &rhs) const = 0;
  virtual bool operator!=(Expr &rhs) const = 0;
  virtual bool operator==(Literal &rhs) const = 0;
  virtual bool operator!=(Literal &rhs) const = 0;
  virtual atl::string getLiteral() const = 0;
};

} // namespace ACC

#endif