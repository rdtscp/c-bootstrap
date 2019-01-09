#pragma once

#ifndef ACC_POSITION_H
#define ACC_POSITION_H

#include <string>

namespace ACC {

class Position final {

public:
  int column;
  int line;
  std::string file;

  Position() {}

  Position(int line, int column, std::string file)
      : line(line), column(column), file(file) {}

  std::string toString() {
    return file + ":" + std::to_string(line) + ":" + std::to_string(column);
  }
};

}; // namespace ACC

#endif