#pragma once

#ifndef SCANNER_H
#define SCANNER_H

#include <string>

namespace ACC {

class Scanner {

public:
  int column;
  int line;

  Scanner(const std::string &filename);

  char next();

  char peek();

private:
  std::string::const_iterator currChar;
  std::string file;
};

}; // namespace ACC

#endif