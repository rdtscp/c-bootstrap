#pragma once

#ifndef BLOCK_H
#define BLOCK_H

#include "Stmt.h"

namespace ACC {

class Block : public Stmt {

public:
  std::vector<std::shared_ptr<Stmt>> blockStmts;

  Block(std::vector<std::shared_ptr<Stmt>> blockStmts)
      : blockStmts(blockStmts) {}

  std::string astClass() const override { return "Block"; }
};

}; // namespace ACC

#endif