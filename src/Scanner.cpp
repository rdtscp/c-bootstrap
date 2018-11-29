#include <fstream>

#include "../include/Scanner.h"

using namespace ACC;

Scanner::Scanner(const std::string &filename, const std::string &filepath)
    : column(1), line(1), filename(filename), filepath(filepath) {
  std::ifstream t(filepath + filename);
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
  includeScanner = newIncludeScanner;
}

std::string Scanner::getFile() const { return file; }

std::string Scanner::getFilename() const { return filename; }

std::string Scanner::getFilepath() const { return filepath; }