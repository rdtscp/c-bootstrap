#pragma once

#include "atl/include/string.h"

namespace ACC {

class Position final {

public:
  int column;
  int line;
  atl::string file;

  Position() {}

  Position(int column, int line, atl::string file)
      : column(column), line(line), file(file) {}

  atl::string toString() const {
    return file + ":" + atl::to_string(line) + ":" + atl::to_string(column);
  }
};

} // namespace ACC
