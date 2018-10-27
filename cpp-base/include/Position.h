#include <string>

namespace ACC {

class Position {

public:
  Position(int line, int column) : line(line), column(column) {}

  std::string toString() { return line + ":" + column; }

private:
  int line;
  int column;
};

}; // namespace ACC