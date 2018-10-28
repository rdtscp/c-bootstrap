#include <fstream>
#include <streambuf>
#include <string>

#include "../include/Scanner.h"

using namespace ACC;

Scanner::Scanner(const std::string &filename) : column(1), line(1) {
  std::ifstream t(filename);
  file = std::string((std::istreambuf_iterator<char>(t)),
                     std::istreambuf_iterator<char>());
  currChar = file.begin();
}

char Scanner::next() {
  char nextChar = *currChar;
  if (nextChar == '\n' || nextChar == '\r') {
    line++;
    column = 0;
  } else {
    column++;
  }
  currChar++;
  return nextChar;
}

char Scanner::peek() { return *currChar; }