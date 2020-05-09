#pragma once

#include "atl/include/shared_ptr.h"
#include "atl/include/string.h"

#include "Position.h"
#include "SourceHandler.h"

namespace ACC {

class Scanner {

public:
  Scanner(const atl::shared_ptr<SourceHandler> &src);

  Scanner(const Scanner &rhs) = delete;

  Scanner operator=(const Scanner &rhs) = delete;

  virtual ~Scanner() {}

  virtual char next();

  char peek() const;

  static atl::string getFilename(const atl::string &abspath);

  static atl::string getFilepath(const atl::string &abspath);

  Position getPosition() const;

protected:
  atl::string::const_iterator currChar;
  int column;
  int line;
  atl::string file;

  atl::string filepath;
  atl::string filename;

  void updateCurrFile();
}; // namespace ACC

} // namespace ACC
