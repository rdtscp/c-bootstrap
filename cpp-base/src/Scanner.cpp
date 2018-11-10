#include <fstream>

#include "../include/Scanner.h"

using namespace ACC;

Scanner::Scanner(const std::string &filename)
    : column(1), line(1)
  file = std::string((std::istreambuf_iterator<char>(t)),
                     std::istreambuf_iterator<char>());
  currChar = file.begin();
}

char Scanner::next() {
  char nextChar = *currChar;
  if ((currChar - 1) == file.end())
    throw std::runtime_error(
        "Internal Error: Attempted to fetch character past EOF.");
  if (nextChar == '\n' || nextChar == '\r') {
    line++;
    column = 0;
  } else {
    column++;
  }
  currChar++;
  return nextChar;
}

char Scanner::peek() {
  if ((currChar - 1) == file.end())
    throw std::runtime_error(
        "Internal Error: Attempted to peek character past EOF.");
  return *currChar;
}