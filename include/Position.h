#pragma once

#ifndef POSITION_H
#define POSITION_H

#include <string>

namespace ACC {

class Position {

public:
  Position(int line, int column, std::string file) : line(line), column(column), file(file) {}

  std::string toString() {
    std::string output = std::to_string(line) + ":" + std::to_string(column);
    return output;
  }

private:
  int column;
  int line;
  std::string file;
};

}; // namespace ACC

#endif