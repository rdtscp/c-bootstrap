#pragma once

#ifndef SCANNER_H
#define SCANNER_H

#include <memory>
#include <string>
#include <vector>

#include "Position.h"

namespace ACC {

class Scanner {

public:
    Scanner(const std::string &abspath);

  char next();

  char peek();

  void startIncluding(const std::shared_ptr<Scanner> &newIncludeScanner);

  std::string getFileContents() const;

  std::string getFilename() const;

  std::string getFilepath() const;

  Position getPosition() const;

private:
  std::string::const_iterator currChar;
  int column;
  int line;
  std::string file;
  const std::string abspath;
  std::shared_ptr<Scanner> includeScanner = nullptr;
};

}; // namespace ACC

#endif