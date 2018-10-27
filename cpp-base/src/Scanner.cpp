#include <fstream>
#include <streambuf>
#include <string>

#include "../include/Scanner.h"

ACC::Scanner::Scanner(const std::string &filename) : column(1), line(1) {
  std::ifstream t(filename);
  file = std::string((std::istreambuf_iterator<char>(t)),
                     std::istreambuf_iterator<char>());
  currChar = file.begin();
}

char ACC::Scanner::next() {
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

char ACC::Scanner::peek() { return *currChar; }