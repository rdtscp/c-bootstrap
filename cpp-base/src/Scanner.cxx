#include <fstream>
#include <streambuf>
#include <string>

#include "Scanner.h"

ACC::Scanner::Scanner(const std::string &filename) : column(1), line(1) {
  std::ifstream t(filename);
  std::string m_file((std::istreambuf_iterator<char>(t)),
                     std::istreambuf_iterator<char>());
  currChar = m_file.begin();
}

char ACC::Scanner::next() {
  if (currChar == file.end())
    return (char)-1;

  char nextChar = *(currChar++);
  if (nextChar == '\n' || nextChar == '\r') {
    line++;
    column = 0;
  } else {
    column++;
  }
  return nextChar;
}

char ACC::Scanner::peek() { return *(currChar + 1); }