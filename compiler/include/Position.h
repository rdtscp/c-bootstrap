#pragma once

#ifndef ACC_POSITION_H
#define ACC_POSITION_H

#include "atl/include/string.h"

namespace ACC {

class Position final {

public:
  int column;
  int line;
  atl::string file;

  Position() {}

  Position(int line, int column, atl::string file)
      : line(line), column(column), file(file) {}

  atl::string toString() {
    return file + ":" + atl::to_string(line) + ":" + atl::to_string(column);
  }
};

}; // namespace ACC

#endif