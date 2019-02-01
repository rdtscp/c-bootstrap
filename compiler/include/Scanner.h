#pragma once

#ifndef ACC_SCANNER_H
#define ACC_SCANNER_H

#include <memory>
#include <string>

#include "Position.h"
#include "SourceHandler.h"

namespace ACC {

class Scanner final {

public:
  Scanner(const SourceHandler &src);

  Scanner(const Scanner &rhs) = delete;

  Scanner operator=(const Scanner &rhs) = delete;

  char next();

  char peek();

  SourceHandler getFileContents() const;

  atl::string getFilename() const;

  atl::string getFilepath() const;

  Position getPosition() const;

private:
  std::string::const_iterator currChar;
  int column;
  int line;
  std::string file;

  std::string abspath;

  void updateCurrFile();
};

}; // namespace ACC

#endif