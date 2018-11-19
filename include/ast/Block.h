#pragma once

#ifndef BLOCK_H
#define BLOCK_H

#include "Stmt.h"

namespace ACC {

class Block : public Stmt {

public:
  Block(std::vector<std::shared_ptr<Stmt>> blockStmts) {
    // Boilerplate Code.
  }
};

}; // namespace ACC

#endif