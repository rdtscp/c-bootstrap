#include "Preprocessor.h"

using namespace ACC;

/* ---- PPScanner ---- */

PPScanner::PPScanner(const SourceHandler &src) : Scanner(src) {}

char PPScanner::next() {
  char nextChar = *currChar;
  if (currChar == file.end() || nextChar == '\0')
    return '\0';

  if (nextChar == '\n' || nextChar == '\r') {
    ++line;
    column = 1;
  } else {
    ++column;
  }
  ++currChar;
  return nextChar;
}

/* ---- Preprocessor ---- */

Preprocessor::Preprocessor(const SourceHandler &src,
                           const atl::vector<atl::string> &includePaths)
    : includePaths(includePaths) {
  fileScanners.push_back(atl::shared_ptr<PPScanner>(new PPScanner(src)));
}

SourceHandler Preprocessor::getSource() {
  char c = fileScanners.top()->next();
  return SourceHandler(SourceHandler::Type::RAW, " ");
}