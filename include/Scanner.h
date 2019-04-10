#pragma once

#include "atl/include/string.h"
#include <string>

#include "Position.h"
#include "SourceHandler.h"

namespace ACC {

class Scanner {

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
  atl::string::const_iterator currChar;
  int column;
  int line;
  atl::string file;

  atl::string abspath;

  void updateCurrFile();
};

} // namespace ACC
