#pragma once

#ifndef DECL_H
#define DECL_H

namespace ACC {

class Decl {

public:
  Decl() {
    // Boilerplate Code.
  }

  bool operator==(const Decl &d) const { return false; }

  virtual std::string toString() const { return "Decl"; }

  virtual std::string strVal() const { return "Decl"; }

};

}; // namespace ACC

#endif