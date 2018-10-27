#pragma once

#include <string>

namespace ACC {

class Position {

public:
  Position(int line, int column) : line(line), column(column) {}

  std::string toString() {
    std::string output = std::to_string(line) + ":" + std::to_string(column);
    return output;
  }

private:
  int line;
  int column;
};

}; // namespace ACC