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

  Scanner(const Scanner &rhs) = delete;

  char next();

  char peek();

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
};

}; // namespace ACC

#endif