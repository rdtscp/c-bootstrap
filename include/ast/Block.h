#pragma once

#ifndef BLOCK_H
#define BLOCK_H

#include "Stmt.h"

namespace ACC {

class Block : public Stmt {

public:

  std::vector<std::shared_ptr<Stmt>> blockStmts;

  Block(std::vector<std::shared_ptr<Stmt>> blockStmts) : blockStmts(blockStmts) {
    // Boilerplate Code.
  }

  std::string toString() const override {
    std::string output;
    output += "{ ";
    for (const std::shared_ptr<Stmt>& stmt : blockStmts)
      output += stmt->toString();
    output += " }";
    return output;
  }

  std::string strVal() const override { return "Block"; }

};

}; // namespace ACC

#endif