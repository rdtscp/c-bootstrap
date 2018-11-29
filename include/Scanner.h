#pragma once

#ifndef SCANNER_H
#define SCANNER_H

#include <memory>
#include <string>
#include <vector>

namespace ACC {

class Scanner {

public:
  int column;
  int line;

  Scanner(const std::string &abspath);

  char next();

  char peek();

  void startIncluding(const std::shared_ptr<Scanner> &newIncludeScanner);

  std::string getFile() const;

  std::string getFilename() const;

  std::string getFilepath() const;

private:
  std::string::const_iterator currChar;
  std::string file;
  const std::string abspath;
  std::shared_ptr<Scanner> includeScanner = nullptr;
};

}; // namespace ACC

#endif