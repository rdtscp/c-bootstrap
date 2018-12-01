#include <fstream>

#include "../include/Scanner.h"

using namespace ACC;

Scanner::Scanner(const std::string &abspath)
    : column(1), line(1), abspath(abspath) {
  std::ifstream t(abspath);
  file = std::string((std::istreambuf_iterator<char>(t)),
                     std::istreambuf_iterator<char>());
  currChar = file.begin();
}

char Scanner::next() {
  if (includeScanner != nullptr) {
    char nextChar = includeScanner->next();
    if (nextChar != '\0')
      return nextChar;

    includeScanner.reset();
  }
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
  if (includeScanner != nullptr) {
    char peek = includeScanner->peek();
    if (peek != '\0')
      return peek;
  }
  if (currChar == file.end())
    return '\0';
  return *currChar;
}

void Scanner::startIncluding(
    const std::shared_ptr<Scanner> &newIncludeScanner) {
  if (includeScanner != nullptr)
    includeScanner->startIncluding(newIncludeScanner);
  else
    includeScanner = newIncludeScanner;
}

std::string Scanner::getFile() const {
  if (includeScanner)
    return includeScanner->file;
  return file;
}

std::string Scanner::getFilename() const {
  if (includeScanner)
    return includeScanner->getFilename();

  std::vector<std::string> directories;
  std::string currDir;
  for (const char currChar : abspath) {
    if (currChar == '/') {
      directories.push_back(currDir);
    } else {
      currDir += currChar;
    }
  }
  directories.push_back(currDir);

  return directories[directories.size() - 1];
}

std::string Scanner::getFilepath() const {
  if (includeScanner)
    return includeScanner->getFilepath();

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