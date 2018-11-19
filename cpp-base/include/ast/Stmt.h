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

  bool operator==(const Stmt &d) const { return true; }

  virtual std::string toString() const { return "Stmt"; }
};

}; // namespace ACC

#endif