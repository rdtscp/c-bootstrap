#pragma once

#ifndef ACC_POSITION_H
#define ACC_POSITION_H

#include "atl/include/string.h"
#include <string>

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
    return file + ":" + std::to_string(line).c_str() + ":" +
           std::to_string(column).c_str();
  }
};

}; // namespace ACC

#endif