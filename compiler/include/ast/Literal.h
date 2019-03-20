#pragma once




#include "Expr.h"

namespace ACC {

class Literal : public Expr {
public:
  const atl::string value;

  Literal(const atl::string &p_value) : value(p_value) {}
  virtual ~Literal() {}
  virtual bool operator==(Expr &rhs) const = 0;
  virtual bool operator!=(Expr &rhs) const = 0;
  virtual bool operator==(Literal &rhs) const = 0;
  virtual bool operator!=(Literal &rhs) const = 0;
  virtual atl::string getLiteral() const = 0;
};

} // namespace ACC

