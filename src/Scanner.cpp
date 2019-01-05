#include <fstream>
#include <iostream>

#include "../include/Scanner.h"

using namespace ACC;

Scanner::Scanner(const std::string &abspath)
    : column(1), line(1), abspath(abspath) {
  std::ifstream t(abspath);
  if (!t.good())
    throw std::runtime_error("Scanner: Provided filename \"" + abspath +
                             "\" could not be read.");

  file = std::string((std::istreambuf_iterator<char>(t)),
                     std::istreambuf_iterator<char>());
  currChar = file.begin();
}

char Scanner::next() {
  char nextChar = *currChar;
  if (currChar == file.end() || nextChar == '\0')
    return '\0';

  if (nextChar == '\n' || nextChar == '\r') {
    line++;
    column = 1;
  } else {
    column++;
  }
  currChar++;
  return nextChar;
}

char Scanner::peek() {
  if (currChar == file.end())
    return '\0';
  return *currChar;
}

std::string Scanner::getFileContents() const { return file; }

std::string Scanner::getFilename() const {
  std::vector<std::string> directories;
  std::string currDir;
  for (const char currChar : abspath) {
    if (currChar == '/') {
      directories.push_back(currDir);
      currDir = "";
    } else {
      currDir += currChar;
    }
  }
  directories.push_back(currDir);

  return directories[directories.size() - 1];
}

std::string Scanner::getFilepath() const {
  std::vector<std::string> directories;
  std::string currDir;
  for (const char currChar : abspath) {
    if (currChar == '/') {
      directories.push_back(currDir);
      currDir = "";
    } else {
      currDir += currChar;
    }
  }
  directories.push_back(currDir);

  std::string filepath;
  for (int i = 0; i < directories.size() - 1; i++)
    filepath += directories[i] + '/';

  return filepath;
}

Position Scanner::getPosition() const {
  return Position(line, column, getFilepath() + getFilename());
}
