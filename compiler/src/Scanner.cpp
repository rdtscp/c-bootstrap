#include <fstream>
#include <iostream>

#include "../include/Scanner.h"

using namespace ACC;

Scanner::Scanner(const SourceHandler &src) : column(1), line(1) {
  if (src.type == SourceHandler::Type::FILEPATH) {
    std::ifstream fileStream(src.value);
    if (!fileStream.good())
      throw std::runtime_error("Scanner: Provided filename \"" + abspath +
                               "\" could not be read.");

    file = std::string((std::istreambuf_iterator<char>(fileStream)),
                       std::istreambuf_iterator<char>());
  } else if (src.type == SourceHandler::Type::RAW) {
    file = src.value;
  }

  currChar = file.begin();
}

char Scanner::next() {
  char nextChar = *currChar;
  if (currChar == file.end() || nextChar == '\0')
    return '\0';

  if (nextChar == '#') {
    currChar++;
    updateCurrFile();
    return next();
  }

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

SourceHandler Scanner::getFileContents() const {
  return SourceHandler(SourceHandler::Type::RAW, file);
}

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

void Scanner::updateCurrFile() {
  char nextChar = next(); // Skip space.

  nextChar = next();

  std::string lineNumStr(1, nextChar);
  while (!std::isspace(peek())) {
    lineNumStr += next();
  }
  unsigned int lineNum = std::stoul(lineNumStr, nullptr, 10);

  nextChar = next(); // Skip space;
  nextChar = next(); // Skip quote;

  std::string filename;
  if (peek() != '<') {
    while (peek() != '\"') {
      filename += next();
    }
  }
  while (peek() != '\n') {
    nextChar = next();
  }
  abspath = filename;
  line = lineNum;
  column = 1;
}