#pragma once

#ifndef STMT_H
#define STMT_H

#include <string>

namespace ACC {

class Stmt {

public:
  Stmt() {
    // Boilerplate Code.
  }

  bool operator==(const Stmt &s) const { return false; }

  virtual std::string toString() const { return "Stmt"; }

  virtual std::string strVal() const { return "Stmt"; }
};

}; // namespace ACC

#endif