#pragma once

#ifndef DECL_H
#define DECL_H

namespace ACC {

class Decl {

public:
  Decl() {
    // Boilerplate Code.
  }

  bool operator==(const Decl &d) const { return true; }

  virtual std::string toString() const { return "Decl"; }
};

}; // namespace ACC

#endif