#pragma once

#ifndef LITERAL_H
#define LITERAL_H

#include "Expr.h"

namespace ACC {

class Literal : public Expr {
public:
  virtual std::string getLiteral() const = 0;
};

}; // namespace ACC

#endif