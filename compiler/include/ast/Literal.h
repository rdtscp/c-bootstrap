#pragma once

#ifndef ACC_LITERAL_H
#define ACC_LITERAL_H

#include "Expr.h"

namespace ACC {

class Literal : public Expr {
public:
  virtual atl::string getLiteral() const = 0;
};

}; // namespace ACC

#endif